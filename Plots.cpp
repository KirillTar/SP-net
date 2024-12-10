#include "Plots.hpp"
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <assert.h>

namespace plt {
	TTF_Font* font;

	constexpr static int h_w = 570;
	constexpr static int h_h = 240;

	SDL_Window* h_win;
	SDL_Renderer* h_ren;

	constexpr static int hd_w = 570;
	constexpr static int hd_h = 240;

	SDL_Window* hd_win;
	SDL_Renderer* hd_ren;

	constexpr static int a_w = 400;
	constexpr static int a_h = 400;
	SDL_Window* a_win;
	SDL_Renderer* a_ren;

	constexpr static int l_w = 315;
	constexpr static int l_h = 315;
	SDL_Window* l_win;
	SDL_Renderer* l_ren;

	void init() {
		SDL_Init(NULL);
		TTF_Init();
		font = TTF_OpenFont("sample.ttf", 15);
		h_win = SDL_CreateWindow("histogramm", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, h_w, h_h, SDL_WINDOW_ALLOW_HIGHDPI);
		h_ren = SDL_CreateRenderer(h_win, -1, SDL_RENDERER_ACCELERATED);
		hd_win = SDL_CreateWindow("histogramm_10", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, hd_w, hd_h, SDL_WINDOW_ALLOW_HIGHDPI);
		hd_ren = SDL_CreateRenderer(hd_win, -1, SDL_RENDERER_ACCELERATED);
		a_win = SDL_CreateWindow("autocorrelation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, a_w, a_h, SDL_WINDOW_ALLOW_HIGHDPI);
		a_ren = SDL_CreateRenderer(a_win, -1, SDL_RENDERER_ACCELERATED);
		l_win = SDL_CreateWindow("lattice", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, l_w, l_h, SDL_WINDOW_ALLOW_HIGHDPI);
		l_ren = SDL_CreateRenderer(l_win, -1, SDL_RENDERER_ACCELERATED);
	}

	void close() {
		TTF_CloseFont(font);
		SDL_DestroyRenderer(h_ren);
		SDL_DestroyRenderer(a_ren);
		SDL_DestroyRenderer(l_ren);
		SDL_DestroyWindow(h_win);
		SDL_DestroyWindow(a_win);
		SDL_DestroyWindow(l_win);
	}

	void draw_value(SDL_Renderer* ren, int value, int x, int y) {

		SDL_Surface* textSurface = NULL;
		SDL_Texture* texture = NULL;
		SDL_Rect textLocation = { 0, 0, 0, 0 };
		SDL_Color clr = { 0, 0, 0, 255 };

		assert(font != nullptr, "Cant open font");

		std::stringstream ss;
		ss << std::scientific << value;

		textSurface = TTF_RenderText_Blended(font, ss.str().c_str(), clr);
		texture = SDL_CreateTextureFromSurface(ren, textSurface);
		textLocation = { int(x - textSurface->w / 2), int(y - textSurface->h / 2), textSurface->w, textSurface->h };
		SDL_RenderCopy(ren, texture, NULL, &textLocation);
		SDL_DestroyTexture(texture);
		SDL_FreeSurface(textSurface);
	}

	void draw_value(SDL_Renderer* ren, double value, int x, int y) {

		SDL_Surface* textSurface = NULL;
		SDL_Texture* texture = NULL;
		SDL_Rect textLocation = { 0, 0, 0, 0 };
		SDL_Color clr = { 0, 0, 0, 255 };

		assert(font != nullptr, "Cant open font");

		std::stringstream ss;
		ss << value;

		textSurface = TTF_RenderText_Blended(font, ss.str().c_str(), clr);
		texture = SDL_CreateTextureFromSurface(ren, textSurface);
		textLocation = { int(x - textSurface->w / 2), int(y - textSurface->h / 2), textSurface->w, textSurface->h };
		SDL_RenderCopy(ren, texture, NULL, &textLocation);
		SDL_DestroyTexture(texture);
		SDL_FreeSurface(textSurface);
	}

	void histogramm(std::vector<uchar>& data) {

		std::vector<double> vec = histogramm_test(data);

		int max = 0;
		for (auto num : vec) {
			if (num > max)
				max = num;
		}

		SDL_SetRenderDrawColor(h_ren, 255, 255, 255, 255);
		SDL_RenderClear(h_ren);

		int x_offset = 40;
		int y_offset = 20;

		SDL_SetRenderDrawColor(h_ren, 0, 0, 0, 255);
		SDL_RenderDrawLine(h_ren, x_offset, y_offset, x_offset, h_h - y_offset);
		SDL_RenderDrawLine(h_ren, x_offset, h_h - y_offset, h_w - x_offset, h_h - y_offset);

		int j = 0;
		for (int i = h_h - y_offset; i >= y_offset; i -= (h_h - 2 * y_offset) / 10) {
			draw_value(h_ren, j * (max / 10), x_offset - 20, i);
			j++;
		}

		j = 0;
		for (int i = x_offset; i <= h_w - x_offset; i += (h_w - 2 * x_offset) / 15) {
			draw_value(h_ren, j * (255 / 15), i, h_h - y_offset + 10);
			j++;
		}

		SDL_SetRenderDrawColor(h_ren, 255, 0, 0, 255);
		j = 0;
		for (double i = x_offset + 1; i <= h_w - x_offset + 1; i += 2) {
			double length = h_h - y_offset - (h_h - 2 * y_offset) * (vec[j] / max);
			SDL_RenderDrawLine(h_ren, i, h_h - y_offset, i, length);
			j++;
			if (j == 256)
				break;
		}

		SDL_RenderPresent(h_ren);
	}

	void histogramm_step_10(std::vector<uchar>& data) {

		std::vector<double> vec = histogramm_test_10(data);

		int max = 0;
		for (auto num : vec) {
			if (num > max)
				max = num;
		}

		SDL_SetRenderDrawColor(hd_ren, 255, 255, 255, 255);
		SDL_RenderClear(hd_ren);

		int x_offset = 40;
		int y_offset = 20;

		SDL_SetRenderDrawColor(hd_ren, 0, 0, 0, 255);
		SDL_RenderDrawLine(hd_ren, x_offset, y_offset, x_offset, hd_h - y_offset);
		SDL_RenderDrawLine(hd_ren, x_offset, hd_h - y_offset, hd_w - x_offset, hd_h - y_offset);

		int j = 0;
		for (int i = hd_h - y_offset; i >= y_offset; i -= (hd_h - 2 * y_offset) / 10) {
			draw_value(hd_ren, j * (max / 10), x_offset - 20, i);
			j++;
		}

		j = 0;
		for (int i = x_offset; i <= hd_w - x_offset; i += (hd_w - 2 * x_offset) / 10) {
			draw_value(hd_ren, j * (255 / 10), i, hd_h - y_offset + 10);
			j++;
		}

		SDL_SetRenderDrawColor(hd_ren, 255, 0, 0, 255);
		j = 0;
		for (double i = x_offset + 1; i <= hd_w - x_offset + 1; i += (hd_w - 2 * x_offset) / 25) {
			double length = hd_h - y_offset - (hd_h - 2 * y_offset) * (vec[j] / max);
			SDL_Rect rect = {i, length, 15, hd_h - length - y_offset + 1};
			SDL_SetRenderDrawColor(hd_ren, 0, 0, 0, 255);
			SDL_RenderDrawRect(hd_ren, &rect);
			SDL_SetRenderDrawColor(hd_ren, 255, 0, 0, 255);
			SDL_RenderFillRect(hd_ren, &rect);
			j++;
		}

		SDL_RenderPresent(hd_ren);
	}

	void autocorrelation(std::vector<uchar>& data, uchar_iter begin_idx, uchar_iter end_idx) {
		std::vector<double> vec = autocorrelation_test(data);

		SDL_SetRenderDrawColor(a_ren, 255, 255, 255, 255);
		SDL_RenderClear(a_ren);

		int x_offset = 30;
		int y_offset = 80;

		SDL_SetRenderDrawColor(a_ren, 0, 0, 0, 255);
		SDL_RenderDrawLine(a_ren, x_offset, y_offset, x_offset, a_h - y_offset);
		SDL_RenderDrawLine(a_ren, x_offset, a_h - y_offset, a_w - x_offset, a_h - y_offset);

		int j = 0;
		for (int i = a_h - y_offset; i >= y_offset; i -= (a_h - 2 * y_offset) / 10) {
			draw_value(a_ren, j * static_cast<double>(1.0 / 10), x_offset - 15, i);
			j++;
		}

		int start = begin_idx - data.begin();
		float distance = end_idx - begin_idx;
		j = 0;

		for (int i = x_offset; i <= a_w - x_offset; i += (a_w - 2 * x_offset) / 5) {
			draw_value(a_ren, j * (distance / 5) + start, i, a_h - y_offset + 65);
			j++;
		}

		SDL_SetRenderDrawColor(a_ren, 255, 0, 0, 255);
		auto iter = vec.begin() + start;
		auto prev = iter;
		iter++;
		double diff = 0;

		for (double i = x_offset + 1; i <= a_w - x_offset + 1; i += 1) {

			while (diff < 1) {
				diff += (a_w - 2 * x_offset) / distance;
				iter++;
			}

			double length = a_h - y_offset - (a_h - 2 * y_offset) * (*prev);
			double length1 = a_h - y_offset - (a_h - 2 * y_offset) * (*iter);
			SDL_RenderDrawLine(a_ren, i - 1, length, i, length1);
			diff = 0;
			prev = iter;
		}

		SDL_RenderPresent(a_ren);
	}

	void lattice(std::vector<uchar>& data) {
		SDL_SetRenderDrawColor(l_ren, 255, 255, 255, 255);
		SDL_RenderClear(l_ren);

		int offset = 30;

		SDL_SetRenderDrawColor(l_ren, 0, 0, 0, 255);
		SDL_RenderDrawLine(l_ren, offset, offset, offset, l_h - offset);
		SDL_RenderDrawLine(l_ren, offset, l_h - offset, l_w - offset, l_h - offset);

		int j = 0;
		for (int i = l_h - offset - 1; i >= offset - 1; i -= (l_h - 2 * offset) / 10) {
			draw_value(l_ren, j * (255 / 10), offset - 15, i);
			j++;
		}

		j = 0;
		for (int i = offset + 1; i <= l_w - offset + 1; i += (l_w - 2 * offset) / 10) {
			draw_value(l_ren, j * (255 / 10), i, l_h - offset + 10);
			j++;
		}

		SDL_SetRenderDrawColor(l_ren, 0, 0, 0, 255);
		for (int i = 0; i < data.size() - 1; i++) {
			SDL_RenderDrawPoint(l_ren, data[i] + 1 + offset, data[i + 1] + 1 + offset);
		}

		SDL_RenderPresent(l_ren);
	}
};




