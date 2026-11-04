#include "ppu.h"
#include "core.h"
#include "cpu.h"
#include "membus.h"


lcd_registers ppu_lcd_registers;

uint8_t frame_pixels[160 * 144];
uint8_t sprite_palette_0[4] = { 0, 1, 2, 3 };
uint8_t sprite_palette_1[4] = { 0, 1, 2, 3 };
uint8_t background_palette[4] = { 0, 1, 2, 3 };
uint16_t ppu_scanline_dot = 0;
uint8_t ppu_pixel_buffer[8] = { 0,0,0,0,0,0,0,0 };
bool ppu_first_frame_after_lcd_enable = false;

uint8_t ppu_oam_buffer_count = 0;
uint8_t ppu_oam_search_index = 0;
oam_buffer_data ppu_oam_buffer[10] = {
    { 0, false }, { 0, false }, { 0, false }, { 0, false }, { 0, false },
    { 0, false }, { 0, false }, { 0, false }, { 0, false }, { 0, false }
};

uint8_t ppu_fetcher_internal_clock = 0;
bool ppu_fetcher_sprite_active = false;
uint8_t ppu_oam_sprite_fetcher_index = 0;
uint16_t ppu_fetcher_tile_no = 0;
bool ppu_fetcher_window_pixels = false;
uint8_t ppu_fetcher_internal_x_pos = 0;
uint8_t ppu_fetcher_window_line_counter = 0;
uint8_t ppu_fetcher_tile_data[2] = { 0,0 };
uint8_t ppu_pixel_fifo_cursor = 0;
pixel_fifo_data ppu_pixel_fifo[16] = {
	{ 0, false }, { 0, false }, { 0, false }, { 0, false },
	{ 0, false }, { 0, false }, { 0, false }, { 0, false },
	{ 0, false }, { 0, false }, { 0, false }, { 0, false },
	{ 0, false }, { 0, false }, { 0, false }, { 0, false },
};
uint8_t ppu_pixel_fifo_occupancy = 0;
bool ppu_fetcher_first_scanline_steps = false;
int16_t ppu_shifter_pixel_x = 0;
uint8_t ppu_discard_pixels = 0;

const SDL_Color ppu_lcd_color_lit = { 223, 230, 179, 255 };
const SDL_Color ppu_gb_colors[4] = {
	{ 185, 200, 90, 255 },
	{ 131, 145, 0, 255 },
	{ 74, 82, 4, 255 },
	{ 40, 44, 4, 255 },
};

const SDL_Color ppu_gb_colors_grey[4] = {
	{ 200, 200, 200, 255 },
	{ 145, 145, 145, 255 },
	{ 82, 82, 82, 255 },
	{ 44, 44, 44, 255 },
};
const SDL_Color ppu_gb_colors_red[4] = {
	{ 200, 90, 90, 255 },
	{ 145, 80, 0, 255 },
	{ 82, 40, 0, 255 },
	{ 44, 0, 0, 255 },
};
const SDL_Color ppu_gb_colors_blue[4] = {
	{ 90, 90, 200, 255 },
	{ 0, 80, 145, 255 },
	{ 0, 40, 82, 255 },
	{ 0, 0, 44, 255 },
};

uint16_t ppu_clocks_spent_in_mode0[154] = { 0 };
uint16_t ppu_clocks_spent_in_mode1[154] = { 0 };
uint16_t ppu_clocks_spent_in_mode2[154] = { 0 };
uint16_t ppu_clocks_spent_in_mode3[154] = { 0 };


void ppu_set_lcd_mode(uint8_t new_mode);
void ppu_fetcher_tick();
void ppu_shifter_tick();
void ppu_check_ly_compare();
void ppu_fetcher_reset();
void render_frame();
void render_debug();


void extract_palette(uint16_t address, uint8_t* palette)
{
    const uint8_t pal = read_mem(address);
    palette[0] = (pal & 0b00000011);
    palette[1] = (pal & 0b00001100) >> 2;
    palette[2] = (pal & 0b00110000) >> 4;
    palette[3] = (pal & 0b11000000) >> 6;
}

void ppu_oam_search_tick()
{
    if (ppu_scanline_dot % 2 == 0 || ppu_oam_buffer_count == 10)
    {
        return;
    }
    const uint8_t* tile_data = VRAM;
    const uint8_t* oam_sprite_data = OAM;
    const uint8_t sprite_height = LCDC_SPRITE_SIZE ? 16 : 8;
    const uint8_t attribute_data_index = ppu_oam_search_index * 4;
    const object_attribute_data* sprite = (object_attribute_data*)(oam_sprite_data + attribute_data_index);
    const int16_t pos_y = sprite->pos_y;
    const uint8_t ly = ppu_lcd_registers.ly + 16;
    if (sprite->pos_x > 0 && ly >= pos_y && ly < pos_y + sprite_height)
    {
        ppu_oam_buffer[ppu_oam_buffer_count].oam_id = attribute_data_index;
        ppu_oam_buffer[ppu_oam_buffer_count].drawn = false;
        ppu_oam_buffer_count++;
    }
    ppu_oam_search_index++;
}










void ppu_init() {
    memset(frame_pixels, 0, 160 * 144);
}

void ppu_reset() {
    ppu_lcd_registers.control = 0x91;
    ppu_lcd_registers.status = 0x85;
    ppu_lcd_registers.ly = 0;
    ppu_lcd_registers.lyc = 0;
    ppu_lcd_registers.oam_dma = 0xFF;
    ppu_lcd_registers.bgp = 0xFC;
    ppu_lcd_registers.obp0 = 0xFF;
    ppu_lcd_registers.obp1 = 0xFF;
    ppu_lcd_registers.wx = 0;
    ppu_lcd_registers.wy = 0;
    ppu_scanline_dot = 0;
    ppu_first_frame_after_lcd_enable = false;
}


void ppu_tick() {
	if (!LCDC_ENABLE_LCD) {
		return;
	}

	if (LCDS_PPU_MODE == OAM_SEARCH) {
		ppu_oam_search_tick();
		if (ppu_scanline_dot == 79) {
			ppu_set_lcd_mode(DRAWING);
		}

	}
	else if (LCDS_PPU_MODE == DRAWING) {
		ppu_fetcher_tick();
		ppu_shifter_tick();
	}

	else if (LCDS_PPU_MODE == H_BLANK) {
		if (ppu_first_frame_after_lcd_enable && ppu_lcd_registers.ly == 0)
		{
			if (ppu_scanline_dot == 79)
			{
				ppu_set_lcd_mode(DRAWING);
			}
			else if (ppu_scanline_dot == 451)
			{
				ppu_scanline_dot = 0;
				ppu_lcd_registers.ly++;
				ppu_check_ly_compare();

			}
		}
		if (ppu_scanline_dot == 455)
		{
			ppu_clocks_spent_in_mode0[ppu_lcd_registers.ly] = ppu_scanline_dot;
			ppu_scanline_dot = 0;
			ppu_lcd_registers.ly++;
			ppu_check_ly_compare();
			if (ppu_lcd_registers.ly == 144)
			{
				ppu_set_lcd_mode(V_BLANK);
				cpu.IF |= 0x01;
			}
			else
			{
				ppu_set_lcd_mode(OAM_SEARCH);
			}
		}

	}
	else if (LCDS_PPU_MODE == V_BLANK) {
		if (ppu_scanline_dot == 455)
		{
			ppu_clocks_spent_in_mode1[ppu_lcd_registers.ly] = ppu_scanline_dot;
			ppu_scanline_dot = 0;
			ppu_lcd_registers.ly++;
			ppu_check_ly_compare();
			if (ppu_lcd_registers.ly == 154)
			{
				ppu_lcd_registers.ly = 0;
				render_frame();
				render_debug();
				ppu_set_lcd_mode(OAM_SEARCH);
				ppu_first_frame_after_lcd_enable = false;
			}
			ppu_check_ly_compare();
		}

	};
	ppu_scanline_dot++;
}



void ppu_check_ly_compare() {
    if (LCDC_ENABLE_LCD && ppu_lcd_registers.ly == ppu_lcd_registers.lyc) {
        ppu_lcd_registers.status |= 0x04;
    }
    else {
        ppu_lcd_registers.status &= 0xFB;
    }
}

void ppu_check_stat_signal() {
    if (!LCDC_ENABLE_LCD) {
        return;
    }

    if ((LCDS_PPU_MODE == OAM_SEARCH && LCDS_MODE_2_STAT_INTERRUPT_ENABLE) ||
        (LCDS_PPU_MODE == H_BLANK && LCDS_MODE_0_STAT_INTERRUPT_ENABLE) ||
        (LCDS_PPU_MODE == V_BLANK && LCDS_MODE_1_STAT_INTERRUPT_ENABLE) ||
        ppu_lcd_registers.ly == ppu_lcd_registers.lyc && LCDS_LYC_EQUALS_LY_INTERRUPT_ENABLE){
        cpu.IF |= 0x02; // LCD Interrupt
        }

}


void ppu_on_lyc_write(uint8_t value) {
    ppu_lcd_registers.lyc = value;
    if (LCDC_ENABLE_LCD) {
        ppu_check_ly_compare();
        ppu_check_stat_signal();
    }
}

void ppu_set_lcd_mode(uint8_t new_mode)
{
    const uint8_t ppu_mode = LCDS_PPU_MODE;
    const uint8_t prev_lcd_status = ppu_lcd_registers.status;
    const uint8_t new_lcd_status = (prev_lcd_status & 0b11111100) | new_mode;
    ppu_lcd_registers.status = new_lcd_status;

	if (new_mode != ppu_mode)
	{
		ppu_check_stat_signal();
		if (new_mode == V_BLANK)
		{
			ppu_fetcher_reset();
		}
		else if (new_mode == OAM_SEARCH)
		{
			ppu_oam_search_index = 0;
			ppu_oam_buffer_count = 0;
		}
		else if (new_mode == DRAWING)
		{
			memset(ppu_pixel_fifo, 0, sizeof(ppu_pixel_fifo));
		}
	}

}


void ppu_on_lcd_control_write(const uint8_t value)
{
    const bool prev_lcd_enabled = LCDC_ENABLE_LCD;
    ppu_lcd_registers.control = value;
    const bool new_lcd_enabled = LCDC_ENABLE_LCD;
    if (new_lcd_enabled != prev_lcd_enabled)
    {
        ppu_scanline_dot = 0;
        ppu_lcd_registers.ly = 0;
        ppu_set_lcd_mode(H_BLANK);

        if (new_lcd_enabled)
        {
            ppu_first_frame_after_lcd_enable = true;
        }
        ppu_check_stat_signal();
    }
}

void ppu_on_lcd_status_write(uint8_t value) {
    uint8_t prev_value = ppu_lcd_registers.status;
    uint8_t new_value = (prev_value & 0b10000111) | (value & 0b01111000);
    ppu_lcd_registers.status = new_value;
    ppu_check_stat_signal();
}


void ppu_on_palette_write(uint16_t addr) {
    if (addr == 0xFF47) extract_palette(addr, background_palette);
    if (addr == 0xFF48) extract_palette(addr, sprite_palette_0);
    if (addr == 0xFF49) extract_palette(addr, sprite_palette_1);
}

void ppu_fetcher_reset()
{
	ppu_fetcher_sprite_active = false;
	ppu_fetcher_internal_clock = 0;
	ppu_fetcher_internal_x_pos = 0;
	ppu_fetcher_window_pixels = false;
	ppu_fetcher_window_line_counter = 0;
	ppu_fetcher_tile_no = 0;
	memset(ppu_fetcher_tile_data, 0, sizeof(ppu_fetcher_tile_data));
	memset(ppu_pixel_fifo, 0, sizeof(ppu_pixel_fifo));
	ppu_pixel_fifo_occupancy = 0;
	ppu_pixel_fifo_cursor = 0;
}

void ppu_fetcher_tick()
{
	ppu_fetcher_internal_clock++;
	// Each fetcher stage takes 2 clock cycles to complete
	if (ppu_fetcher_internal_clock == 2) // Step 1: Fetch Tile Address
	{
		if (ppu_fetcher_sprite_active)
		{
			const uint8_t* oam_sprite_data = OAM;
			const object_attribute_data* sprite = (object_attribute_data*)(oam_sprite_data + ppu_oam_buffer[ppu_oam_sprite_fetcher_index].oam_id);
			ppu_fetcher_tile_no = sprite->tile_id;
		}
		else
		{
			if (!ppu_fetcher_window_pixels || !LCDC_WINDOW_ENABLE)
			{
				const uint8_t* tile_map_data = VRAM + (LCDC_BG_TILE_MAP_SELECT ? 0x1C00 : 0x1800);
				const uint16_t tile_x_offset = (ppu_fetcher_internal_x_pos + (ppu_lcd_registers.scx / 8)) & 0x1F;
				const uint16_t tile_y_offset = 32 * (((ppu_lcd_registers.ly + ppu_lcd_registers.scy) & 0xFF) / 8);
				const uint16_t tile_address = (tile_x_offset + tile_y_offset) & 0x3FF;
				ppu_fetcher_tile_no = tile_map_data[tile_address];
			}
			else
			{
				const uint8_t* tile_map_data = VRAM + (LCDC_WINDOW_TILE_MAP_SELECT ? 0x1C00 : 0x1800);
				const uint16_t tile_x_offset = ppu_fetcher_internal_x_pos;
				const uint16_t tile_y_offset = 32 * (ppu_fetcher_window_line_counter / 8);
				const uint16_t tile_address = (tile_x_offset + tile_y_offset) & 0x3FF;
				ppu_fetcher_tile_no = tile_map_data[tile_address];
			}
		}
	}
	else if (ppu_fetcher_internal_clock == 4 || ppu_fetcher_internal_clock == 6) // Steps 2 and 3: Fetch Tile Data
	{
		const bool high_byte = ppu_fetcher_internal_clock == 6;
		if (ppu_fetcher_sprite_active)
		{
			const uint8_t* oam_sprite_data = OAM;
			const object_attribute_data* sprite = (object_attribute_data*)(oam_sprite_data + ppu_oam_buffer[ppu_oam_sprite_fetcher_index].oam_id);
			const uint8_t* tile_data = VRAM;
			const int32_t tile_data_offset = ppu_fetcher_tile_no * 16;
			const bool flip_y = bit_helper_func(sprite->flags, 6);
			const uint8_t flip_offset = LCDC_SPRITE_SIZE ? 30 : 14;
			const uint8_t vertical_offset = 2 * (ppu_lcd_registers.ly - (sprite->pos_y - 16));
			const uint16_t offset = flip_y ? flip_offset - vertical_offset : vertical_offset;
			const int32_t pixel_data_address = tile_data_offset + offset + (high_byte ? 1 : 0);
			ppu_fetcher_tile_data[high_byte] = tile_data[pixel_data_address];
		}
		else
		{
			const uint8_t* tile_data = VRAM + (LCDC_TILE_DATA_SELECT ? 0x0000 : 0x1000);
			const int32_t tile_index = LCDC_TILE_DATA_SELECT ? ppu_fetcher_tile_no : (int8_t)ppu_fetcher_tile_no;
			const int32_t tile_data_offset = tile_index * 16;
			if (!ppu_fetcher_window_pixels)
			{
				const uint16_t offset = 2 * ((ppu_lcd_registers.ly + ppu_lcd_registers.scy) % 8);
				const int32_t pixel_data_address = tile_data_offset + offset + (high_byte ? 1 : 0);
				ppu_fetcher_tile_data[high_byte] = tile_data[pixel_data_address];
			}
			else
			{
				const uint16_t offset = 2 * (ppu_fetcher_window_line_counter % 8);
				const int32_t pixel_data_address = tile_data_offset + offset + (high_byte ? 1 : 0);
				ppu_fetcher_tile_data[high_byte] = tile_data[pixel_data_address];
			}
		}
	}
	if (ppu_fetcher_internal_clock >= 6) // Step 4: Push to pixel FIFO
	{
		if (ppu_fetcher_sprite_active)
		{
			const uint8_t* oam_sprite_data = OAM;
			const object_attribute_data* sprite = (object_attribute_data*)(oam_sprite_data + ppu_oam_buffer[ppu_oam_sprite_fetcher_index].oam_id);
			const bool sprite_uses_palette_1 = bit_helper_func(sprite->flags, 4);
			const bool sprite_flip_x = bit_helper_func(sprite->flags, 5);
			const bool sprite_bg_priority = bit_helper_func(sprite->flags, 7);
			for (uint8_t column = 0; column < 8; column++)
			{
				const uint8_t right_bit = (ppu_fetcher_tile_data[0] & (0b10000000 >> column)) >> (7 - column);
				const uint8_t left_bit = (ppu_fetcher_tile_data[1] & (0b10000000 >> column)) >> (7 - column);
				const uint8_t pixel_value_sprite = ((left_bit << 1) | right_bit);
				const uint8_t index = (ppu_pixel_fifo_cursor + (sprite_flip_x ? 7 - column : column)) % 16;
				if (pixel_value_sprite > 0 && !ppu_pixel_fifo[index].from_sprite &&
					!(sprite_bg_priority && ppu_pixel_fifo[index].color > 0)) // Blend sprite pixels with pixel FIFO
				{
					const uint8_t paletted_pixel_value_sprite = sprite_uses_palette_1 ? sprite_palette_1[pixel_value_sprite] : sprite_palette_0[pixel_value_sprite];
					ppu_pixel_fifo[index].color = paletted_pixel_value_sprite;
					ppu_pixel_fifo[index].from_sprite = true;
				}
			}
			ppu_oam_buffer[ppu_oam_sprite_fetcher_index].drawn = true;
			ppu_fetcher_internal_clock = 0; // Succesful push to FIFO
			ppu_fetcher_sprite_active = false;
		}
		else if (ppu_pixel_fifo_occupancy <= 8)
		{
			for (uint8_t column = 0; column < 8; column++)
			{
				const uint8_t index = (ppu_pixel_fifo_cursor + ppu_pixel_fifo_occupancy) % 16;
				const uint8_t right_bit = (ppu_fetcher_tile_data[0] & (0b10000000 >> column)) >> (7 - column);
				const uint8_t left_bit = (ppu_fetcher_tile_data[1] & (0b10000000 >> column)) >> (7 - column);
				const uint8_t pixel_value_bg = ((left_bit << 1) | right_bit);
				ppu_pixel_fifo[index].color = LCDC_BG_WINDOW_ENABLE ? pixel_value_bg : 0;
				ppu_pixel_fifo[index].from_sprite = false;
				ppu_pixel_fifo_occupancy++;
			}
			ppu_fetcher_internal_clock = 0; // Succesful push to FIFO
			if (!ppu_fetcher_first_scanline_steps)
			{
				ppu_fetcher_internal_x_pos++;
			}
			else
			{
				ppu_fetcher_first_scanline_steps = false;
			}
		}
	}
}




void ppu_shifter_tick()
{
	if (ppu_fetcher_sprite_active)
	{
		return; // Can't shift pixels whilst sprite fetching is active
	}
	if (ppu_pixel_fifo_occupancy <= 8)
	{
		return; // Can't shift pixels unless there are at least 9 in the FIFO
	}
	if (LCDC_SPRITE_ENABLE && ppu_oam_buffer_count > 0)
	{
		const uint8_t* oam_sprite_data = OAM;
		for (uint8_t i = 0; i < 10; i++)
		{
			if (ppu_oam_buffer[i].drawn) continue;
			const object_attribute_data* sprite = (object_attribute_data*)(oam_sprite_data + ppu_oam_buffer[i].oam_id);
			if (sprite->pos_x <= ppu_shifter_pixel_x + 8 &&
				sprite->pos_x > ppu_shifter_pixel_x)
			{
				ppu_oam_sprite_fetcher_index = i;
				ppu_fetcher_internal_clock = 0; // reset fetcher step
				ppu_fetcher_sprite_active = true;
				return;
			}
		}
	}
	if (ppu_discard_pixels > 0 && !ppu_fetcher_window_pixels) // SCX handling
	{
		ppu_discard_pixels--;
		ppu_pixel_fifo_occupancy--;
		ppu_pixel_fifo_cursor++;
		return;
	}
	if (ppu_shifter_pixel_x >= 0) // First 8 pixels are always off-screen and discarded
	{
		const uint8_t pixel_value = ppu_pixel_fifo[ppu_pixel_fifo_cursor].color;
		const uint16_t screen_index = (ppu_lcd_registers.ly * 160) + ppu_shifter_pixel_x;
		if (ppu_pixel_fifo[ppu_pixel_fifo_cursor].from_sprite)
		{
			frame_pixels[screen_index] = pixel_value;
		}
		else
		{
			const uint8_t paletted_pixel_value_bg = background_palette[pixel_value];
			frame_pixels[screen_index] = paletted_pixel_value_bg;
		}
	}

	ppu_shifter_pixel_x++;
	ppu_pixel_fifo_occupancy--;
	ppu_pixel_fifo_cursor = (ppu_pixel_fifo_cursor + 1) % 16;
	if (LCDC_WINDOW_ENABLE &&
		ppu_lcd_registers.wy == ppu_lcd_registers.ly &&
		ppu_shifter_pixel_x >= ppu_lcd_registers.wx - 7 &&
		!ppu_fetcher_window_pixels)
	{
		ppu_fetcher_reset();
		ppu_fetcher_window_pixels = true;
	}

	if (ppu_shifter_pixel_x == 160) // reached H-blank
	{
		ppu_shifter_pixel_x = -8;
		ppu_pixel_fifo_cursor = 0;
		ppu_pixel_fifo_occupancy = 0;
		ppu_fetcher_internal_x_pos = 0;
		ppu_fetcher_internal_clock = 0;
		ppu_fetcher_first_scanline_steps = true;
		if (ppu_fetcher_window_pixels)
		{
			ppu_fetcher_window_line_counter++;
		}
		ppu_set_lcd_mode(H_BLANK);
	}
}


void render_frame() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT) {
			quitting = true;
		}
	}
	if (quitting) {
		return;
	}

	SDL_SetRenderDrawColor(gfx_render_ctx->renderer, ppu_lcd_color_lit.r, ppu_lcd_color_lit.g, ppu_lcd_color_lit.b, ppu_lcd_color_lit.a);
	SDL_RenderClear(gfx_render_ctx->renderer);
	if (!LCDC_ENABLE_LCD || ppu_first_frame_after_lcd_enable)
	{
		SDL_RenderPresent(gfx_render_ctx->renderer);
		return;
	}

	void* pixels = nullptr;
	int pitch = 0;
	if (!SDL_LockTexture(gfx_render_ctx->texture, nullptr, &pixels, &pitch)) {
		Corelog << "Unable to lock render texture: " << SDL_GetError() << '\n';
		SDL_RenderPresent(gfx_render_ctx->renderer);
		return;
	}

	for (uint16_t y = 0; y < LCD_PIXELS_HEIGHT; y++)
	{
		for (uint16_t x = 0; x < LCD_PIXELS_WIDTH; x++)
		{
			const uint32_t index = (y * LCD_PIXELS_WIDTH) + x;
			const uint8_t color_index = frame_pixels[index];
			const SDL_Color& color = ppu_gb_colors[color_index];
			for (uint32_t scale_y = 0; scale_y < PPU_RENDER_SCALE; scale_y++) {
				uint8_t* row = static_cast<uint8_t*>(pixels) +
					((y * PPU_RENDER_SCALE + scale_y) * pitch);
				for (uint32_t scale_x = 0; scale_x < PPU_RENDER_SCALE; scale_x++) {
					uint8_t* pixel = row + ((x * PPU_RENDER_SCALE + scale_x) * 4);
					pixel[0] = color.r;
					pixel[1] = color.g;
					pixel[2] = color.b;
					pixel[3] = color.a;
				}
			}
		}
	}

	SDL_UnlockTexture(gfx_render_ctx->texture);
	SDL_FRect src_rect = {
		0, 0,
		static_cast<float>(LCD_PIXELS_WIDTH * PPU_RENDER_SCALE),
		static_cast<float>(LCD_PIXELS_HEIGHT * PPU_RENDER_SCALE)
	};
	if (!SDL_RenderTexture(gfx_render_ctx->renderer, gfx_render_ctx->texture, &src_rect, nullptr)) {
		Corelog << "Unable to render frame texture: " << SDL_GetError() << '\n';
	}
	SDL_RenderPresent(gfx_render_ctx->renderer);

	memset(frame_pixels, 0, LCD_PIXELS_WIDTH * LCD_PIXELS_HEIGHT);
}

void render_debug() {

}
