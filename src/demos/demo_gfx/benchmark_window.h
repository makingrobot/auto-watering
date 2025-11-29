/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "config.h"
#if APP_DEMO_GFX==1

#ifndef _BENCHMARK_WINDOW_H
#define _BENCHMARK_WINDOW_H

#include <Arduino.h>
#include <Arduino_GFX_Library.h>

#include "src/framework/display/gfx_window.h"
#include "src/framework/sys/log.h"

#define TAG "BenchmarkWindow"

class BenchmarkWindow : public GfxWindow {
public:
    BenchmarkWindow() { }

    void Setup(Arduino_GFX* driver) override {
        gfx = driver;
    }

    void SetStatus(const std::string& status) override { }
    void SetText(const std::string& text) override { }
   
    void Benchmark() {

        w = gfx->width();
        h = gfx->height();
        n = min(w, h);
        n1 = n - 1;
        cx = w / 2;
        cy = h / 2;
        cx1 = cx - 1;
        cy1 = cy - 1;
        cn = min(cx1, cy1);
        cn1 = cn - 1;
        tsa = ((w <= 176) || (h <= 160)) ? 1 : (((w <= 240) || (h <= 240)) ? 2 : 3); // text size A
        tsb = ((w <= 272) || (h <= 220)) ? 1 : 2;                                    // text size B
        tsc = ((w <= 220) || (h <= 220)) ? 1 : 2;                                    // text size C
        ds = (w <= 160) ? 9 : 12;                                                    // digit size

        while (1) {
            Serial.println("Benchmark\tmicro-secs");

            int32_t usecFillScreen = testFillScreen();
            serialOut("Screen fill\t", usecFillScreen, 100, true);

            int32_t usecText = testText();
            serialOut("Text\t", usecText, 3000, true);

            int32_t usecPixels = testPixels();
            serialOut("Pixels\t", usecPixels, 100, true);

            int32_t usecLines = testLines();
            serialOut("Lines\t", usecLines, 100, true);

            int32_t usecFastLines = testFastLines();
            serialOut("Horiz/Vert Lines\t", usecFastLines, 100, true);

            int32_t usecFilledRects = testFilledRects();
            serialOut("Rectangles (filled)\t", usecFilledRects, 100, false);

            int32_t usecRects = testRects();
            serialOut("Rectangles (outline)\t", usecRects, 100, true);

            int32_t usecFilledTrangles = testFilledTriangles();
            serialOut("Triangles (filled)\t", usecFilledTrangles, 100, false);

            int32_t usecTriangles = testTriangles();
            serialOut("Triangles (outline)\t", usecTriangles, 100, true);

            int32_t usecFilledCircles = testFilledCircles(10);
            serialOut("Circles (filled)\t", usecFilledCircles, 100, false);

            int32_t usecCircles = testCircles(10);
            serialOut("Circles (outline)\t", usecCircles, 100, true);

            int32_t usecFilledArcs = testFillArcs();
            serialOut("Arcs (filled)\t", usecFilledArcs, 100, false);

            int32_t usecArcs = testArcs();
            serialOut("Arcs (outline)\t", usecArcs, 100, true);

            int32_t usecFilledRoundRects = testFilledRoundRects();
            serialOut("Rounded rects (filled)\t", usecFilledRoundRects, 100, false);

            int32_t usecRoundRects = testRoundRects();
            serialOut("Rounded rects (outline)\t", usecRoundRects, 100, true);

    #ifdef CANVAS
            uint32_t start = micros_start();
            gfx->flush();
            int32_t usecFlush = micros() - start;
            serialOut("flush (Canvas only)\t", usecFlush, 0, false);
    #endif

            Serial.println("Done!");
            //gfx->fillScreen(BLACK);

            uint16_t c = 4;
            int8_t d = 1;
            for (int32_t i = 0; i < h; i++)
            {
                gfx->drawFastHLine(0, i, w, c);
                c += d;
                if (c <= 4 || c >= 11)
                {
                    d = -d;
                }
            }

            gfx->setCursor(0, 0);

            gfx->setTextSize(tsa);
            gfx->setTextColor(MAGENTA);
            gfx->println("Arduino GFX PDQ");

            if (h > w)
            {
                gfx->setTextSize(tsb);
                gfx->setTextColor(GREEN);
                gfx->print("\nBenchmark ");
                gfx->setTextSize(tsc);
                if (ds == 12)
                {
                    gfx->print("   ");
                }
                gfx->println("micro-secs");
            }

            printnice("Screen fill ", usecFillScreen);
            printnice("Text        ", usecText);
            printnice("Pixels      ", usecPixels);
            printnice("Lines       ", usecLines);
            printnice("H/V Lines   ", usecFastLines);
            printnice("Rectangles F", usecFilledRects);
            printnice("Rectangles  ", usecRects);
            printnice("Triangles F ", usecFilledTrangles);
            printnice("Triangles   ", usecTriangles);
            printnice("Circles F   ", usecFilledCircles);
            printnice("Circles     ", usecCircles);
            printnice("Arcs F      ", usecFilledArcs);
            printnice("Arcs        ", usecArcs);
            printnice("RoundRects F", usecFilledRoundRects);
            printnice("RoundRects  ", usecRoundRects);

            if ((h > w) || (h > 240))
            {
                gfx->setTextSize(tsc);
                gfx->setTextColor(GREEN);
                gfx->print("\nBenchmark Complete!");
            }

    #ifdef CANVAS
            gfx->flush();
    #endif
            delay(60 * 1000L);
        }
    }

private:

    Arduino_GFX* gfx = nullptr;
    int32_t w, h, n, n1, cx, cy, cx1, cy1, cn, cn1;
    uint8_t tsa, tsb, tsc, ds;

    static inline uint32_t micros_start()
    {
        uint8_t oms = millis();
        while ((uint8_t)millis() == oms)
            ;
        return micros();
    }

    void serialOut(const char *item, int32_t v, uint32_t d, bool clear)
    {
#ifdef CANVAS
        gfx->flush();
#endif
        Serial.print(item);
        if (v < 0)
        {
            Serial.println("N/A");
        }
        else
        {
            Serial.println(v);
        }
        delay(d);
        if (clear)
        {
            gfx->fillScreen(BLACK);
        }
    }

    void printnice(const char *item, long int v)
    {
        gfx->setTextSize(tsb);
        gfx->setTextColor(CYAN);
        gfx->print(item);

        gfx->setTextSize(tsc);
        gfx->setTextColor(YELLOW);
        if (v < 0)
        {
            gfx->println("      N / A");
        }
        else
        {
            char str[32] = {0};
        #ifdef RTL8722DM
            sprintf(str, "%d", (int)v);
        #else
            sprintf(str, "%ld", v);
        #endif
            for (char *p = (str + strlen(str)) - 3; p > str; p -= 3)
            {
            memmove(p + 1, p, strlen(p) + 1);
            *p = ',';
            }
            while (strlen(str) < ds)
            {
            memmove(str + 1, str, strlen(str) + 1);
            *str = ' ';
            }
            gfx->println(str);
        }
    }

    int32_t testFillScreen()
    {
        uint32_t start = micros_start();
        // Shortened this tedious test!
        gfx->fillScreen(WHITE);
        gfx->fillScreen(RED);
        gfx->fillScreen(GREEN);
        gfx->fillScreen(BLUE);
        gfx->fillScreen(BLACK);

        return micros() - start;
    }

    int32_t testText()
    {
        uint32_t start = micros_start();
        gfx->setCursor(0, 0);

        gfx->setTextSize(1);
        gfx->setTextColor(WHITE, BLACK);
        gfx->println("Hello World!");

        gfx->setTextSize(2);
        gfx->setTextColor(gfx->color565(0xff, 0x00, 0x00));
        gfx->print("RED ");
        gfx->setTextColor(gfx->color565(0x00, 0xff, 0x00));
        gfx->print("GREEN ");
        gfx->setTextColor(gfx->color565(0x00, 0x00, 0xff));
        gfx->println("BLUE");

        gfx->setTextSize(tsa);
        gfx->setTextColor(YELLOW);
        gfx->println(1234.56);

        gfx->setTextColor(WHITE);
        gfx->println((w > 128) ? 0xDEADBEEF : 0xDEADBEE, HEX);

        gfx->setTextColor(CYAN, WHITE);
        gfx->println("Groop,");

        gfx->setTextSize(tsc);
        gfx->setTextColor(MAGENTA, WHITE);
        gfx->println("I implore thee,");

        gfx->setTextSize(1);
        gfx->setTextColor(NAVY, WHITE);
        gfx->println("my foonting turlingdromes.");

        gfx->setTextColor(DARKGREEN, WHITE);
        gfx->println("And hooptiously drangle me");

        gfx->setTextColor(DARKCYAN, WHITE);
        gfx->println("with crinkly bindlewurdles,");

        gfx->setTextColor(MAROON, WHITE);
        gfx->println("Or I will rend thee");

        gfx->setTextColor(PURPLE, WHITE);
        gfx->println("in the gobberwartsb");

        gfx->setTextColor(OLIVE, WHITE);
        gfx->println("with my blurglecruncheon,");

        gfx->setTextColor(DARKGREY, WHITE);
        gfx->println("see if I don't!");

        gfx->setTextSize(2);
        gfx->setTextColor(RED);
        gfx->println("Size 2");

        gfx->setTextSize(3);
        gfx->setTextColor(ORANGE);
        gfx->println("Size 3");

        gfx->setTextSize(4);
        gfx->setTextColor(YELLOW);
        gfx->println("Size 4");

        gfx->setTextSize(5);
        gfx->setTextColor(GREENYELLOW);
        gfx->println("Size 5");

        gfx->setTextSize(6);
        gfx->setTextColor(GREEN);
        gfx->println("Size 6");

        gfx->setTextSize(7);
        gfx->setTextColor(BLUE);
        gfx->println("Size 7");

        gfx->setTextSize(8);
        gfx->setTextColor(PURPLE);
        gfx->println("Size 8");

        gfx->setTextSize(9);
        gfx->setTextColor(PALERED);
        gfx->println("Size 9");

        return micros() - start;
    }

    int32_t testPixels()
    {
        uint32_t start = micros_start();

        for (int16_t y = 0; y < h; y++)
        {
            for (int16_t x = 0; x < w; x++)
            {
            gfx->drawPixel(x, y, gfx->color565(x << 3, y << 3, x * y));
            }
        }

        return micros() - start;
    }

    int32_t testLines()
    {
        uint32_t start;
        int32_t x1, y1, x2, y2;

        start = micros_start();

        x1 = y1 = 0;
        y2 = h - 1;
        for (x2 = 0; x2 < w; x2 += 6)
        {
            gfx->drawLine(x1, y1, x2, y2, BLUE);
        }

        x2 = w - 1;
        for (y2 = 0; y2 < h; y2 += 6)
        {
            gfx->drawLine(x1, y1, x2, y2, BLUE);
        }

        x1 = w - 1;
        y1 = 0;
        y2 = h - 1;
        for (x2 = 0; x2 < w; x2 += 6)
        {
            gfx->drawLine(x1, y1, x2, y2, BLUE);
        }

        x2 = 0;
        for (y2 = 0; y2 < h; y2 += 6)
        {
            gfx->drawLine(x1, y1, x2, y2, BLUE);
        }

        x1 = 0;
        y1 = h - 1;
        y2 = 0;
        for (x2 = 0; x2 < w; x2 += 6)
        {
            gfx->drawLine(x1, y1, x2, y2, BLUE);
        }

        x2 = w - 1;
        for (y2 = 0; y2 < h; y2 += 6)
        {
            gfx->drawLine(x1, y1, x2, y2, BLUE);
        }

        x1 = w - 1;
        y1 = h - 1;
        y2 = 0;
        for (x2 = 0; x2 < w; x2 += 6)
        {
            gfx->drawLine(x1, y1, x2, y2, BLUE);
        }

        x2 = 0;
        for (y2 = 0; y2 < h; y2 += 6)
        {
            gfx->drawLine(x1, y1, x2, y2, BLUE);
        }

        return micros() - start;
    }

    int32_t testFastLines()
    {
        uint32_t start;
        int32_t x, y;

        start = micros_start();

        for (y = 0; y < h; y += 5)
        {
            gfx->drawFastHLine(0, y, w, RED);
        }
        for (x = 0; x < w; x += 5)
        {
            gfx->drawFastVLine(x, 0, h, BLUE);
        }

        return micros() - start;
    }

    int32_t testFilledRects()
    {
        uint32_t start;
        int32_t i, i2;

        start = micros_start();

        for (i = n; i > 0; i -= 6)
        {
            i2 = i / 2;

            gfx->fillRect(cx - i2, cy - i2, i, i, gfx->color565(i, i, 0));
        }

        return micros() - start;
    }

    int32_t testRects()
    {
        uint32_t start;
        int32_t i, i2;

        start = micros_start();
        for (i = 2; i < n; i += 6)
        {
            i2 = i / 2;
            gfx->drawRect(cx - i2, cy - i2, i, i, GREEN);
        }

        return micros() - start;
    }

    int32_t testFilledCircles(uint8_t radius)
    {
        uint32_t start;
        int32_t x, y, r2 = radius * 2;

        start = micros_start();

        for (x = radius; x < w; x += r2)
        {
            for (y = radius; y < h; y += r2)
            {
            gfx->fillCircle(x, y, radius, MAGENTA);
            }
        }

        return micros() - start;
    }

    int32_t testCircles(uint8_t radius)
    {
        uint32_t start;
        int32_t x, y, r2 = radius * 2;
        int32_t w1 = w + radius;
        int32_t h1 = h + radius;

        // Screen is not cleared for this one -- this is
        // intentional and does not affect the reported time.
        start = micros_start();

        for (x = 0; x < w1; x += r2)
        {
            for (y = 0; y < h1; y += r2)
            {
            gfx->drawCircle(x, y, radius, WHITE);
            }
        }

        return micros() - start;
    }

    int32_t testFillArcs()
    {
        int16_t i, r = 360 / cn;
        uint32_t start = micros_start();

        for (i = 6; i < cn; i += 6)
        {
            gfx->fillArc(cx1, cy1, i, i - 3, 0, i * r, RED);
        }

        return micros() - start;
    }

    int32_t testArcs()
    {
        int16_t i, r = 360 / cn;
        uint32_t start = micros_start();

        for (i = 6; i < cn; i += 6)
        {
            gfx->drawArc(cx1, cy1, i, i - 3, 0, i * r, WHITE);
        }

        return micros() - start;
    }

    int32_t testFilledTriangles()
    {
        uint32_t start;
        int32_t i;

        start = micros_start();

        for (i = cn1; i > 10; i -= 5)
        {
            gfx->fillTriangle(cx1, cy1 - i, cx1 - i, cy1 + i, cx1 + i, cy1 + i,
                            gfx->color565(0, i, i));
        }

        return micros() - start;
    }

    int32_t testTriangles()
    {
        uint32_t start;
        int32_t i;

        start = micros_start();

        for (i = 0; i < cn; i += 5)
        {
            gfx->drawTriangle(
                cx1, cy1 - i,     // peak
                cx1 - i, cy1 + i, // bottom left
                cx1 + i, cy1 + i, // bottom right
                gfx->color565(0, 0, i));
        }

        return micros() - start;
    }

    int32_t testFilledRoundRects()
    {
        uint32_t start;
        int32_t i, i2;

        start = micros_start();

        for (i = n1; i > 20; i -= 6)
        {
            i2 = i / 2;
            gfx->fillRoundRect(cx - i2, cy - i2, i, i, i / 8, gfx->color565(0, i, 0));
        }

        return micros() - start;
    }

    int32_t testRoundRects()
    {
        uint32_t start;
        int32_t i, i2;

        start = micros_start();

        for (i = 20; i < n1; i += 6)
        {
            i2 = i / 2;
            gfx->drawRoundRect(cx - i2, cy - i2, i, i, i / 8, gfx->color565(i, 0, 0));
        }

        return micros() - start;
    }

};

#endif // _BENCHMARK_WINDOW_H

#endif