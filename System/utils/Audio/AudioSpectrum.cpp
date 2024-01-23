#include <utils/Audio/AudioSpectrum.hpp>
#include <Siv3D.hpp>

namespace
{
    Image CreateGrad(int sizeX, int sizeY)
    {
        Image image(sizeX, sizeY);
        const Vec2 center = Vec2(image.width(), image.height()) / 2;

        const double maxLength = center.x;

        Color col;

        for (int y = 0; y < image.height(); ++y) {
            for (int x = 0; x < image.width(); ++x) {
                const Vec2 pos(x, y);

                const double length = (center - pos).length();

                const double value = length / maxLength;

                col = ColorF(1.0, 1.0 - Pow(value, 2.0));

                image[y][x] = col;
            }
        }
        return image;
    }
    double getRad(double freq)
    {
        return 2 * Math::Pi * Log(freq) / Log(2.0) + 2.42;
    }

    float bufferAverage(const float buffer[], int index, int sample)
    {
        index = index > sample / 2 ? index - sample / 2 : index;
        float sum = 0;
        for (auto i : step(sample)) {
            sum += buffer[index + i];
        }
        sum /= sample;
        return sum;
    }
}

namespace ct
{
    void AudioSpectrum::draw(const Audio& sound) const
    {
        static Texture grad(::CreateGrad(10, 10));

        FFTResult fft;
        s3d::FFT::Analyze(fft, sound);
        if (!sound.isPlaying()) {
            return;
        }

        const auto base = bufferAverage(fft.buffer.data(), static_cast<int>(m_baseHz / 5.38), 10);

        ScopedRenderStates2D scoped(BlendState::Additive);
        constexpr int devide = 2;
        for (auto i : step(360 / devide)) {
            const double di = i * devide;

            const double radian = getRad(di);
            const double radianWidth = Max((radian)-getRad(di - 1), 0.0);

            const double width = Clamp(m_radius * (radianWidth), 0.0, 50.0) + Pow(base, 0.8f) * 200;

            const double size = Pow(bufferAverage(fft.buffer.data(), i * devide, devide), 0.6f) * 800 * Log(di) / Log(10.0);

            const double sliderStrength = Pow(10.0, 0.5 * 2.0);

            const double strength = sliderStrength * Pow(size, 2.0) * Log(di) / 10000000.0;

            int32 h = (static_cast<int32>(240 - Log(di) * 100) % 360 + 360) % 360;
            Color col = HSV(h, 1.0, Saturate(strength)).toColor(150);
            const Vec2 posOuter = Circular(m_radius, radian) + Scene::Center();

            grad.resized(width * 2, width * 2).rotated(radian).drawAt(posOuter, col);
        }
    }
}
