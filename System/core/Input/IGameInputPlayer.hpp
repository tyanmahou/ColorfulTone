#pragma once
#include <core/Types.hpp>
#include <Siv3D/InputGroup.hpp>

namespace ct
{
    enum class GameInputKind
    {
        Red,
        Blue,
        Yellow,
    };

    /// <summary>
    /// 入力抽象クラス
    /// </summary>
    class IGameInputPlayer
    {
    public:
        virtual ~IGameInputPlayer() = default;

        /// <summary>
        /// クリック判定
        /// </summary>
        /// <param name="kind"></param>
        /// <returns></returns>
        virtual bool isClicked(GameInputKind kind) const = 0;

        /// <summary>
        /// Pressed判定
        /// </summary>
        /// <param name="kind"></param>
        /// <returns></returns>
        virtual bool isPressed(GameInputKind kind)  const = 0;

        /// <summary>
        /// 入力のポップ
        /// </summary>
        /// <param name="kind"></param>
        virtual void pop(GameInputKind kind) = 0;

        /// <summary>
        /// クリックオフセット
        /// </summary>
        /// <param name="kind"></param>
        /// <returns></returns>
        virtual s3d::int64 clickedMillisecOffset(GameInputKind kind)  const = 0;

        /// <summary>
        /// 強制入力
        /// </summary>
        /// <param name="kind"></param>
        virtual void forceInput(GameInputKind kind, bool isLong) = 0;

        /// <summary>
        /// 更新
        /// </summary>
        virtual void update() = 0;
    };

    [[nodiscard]] const s3d::InputGroup& GetPlayKey(GameInputKind kind);
    [[nodiscard]] const s3d::InputGroup& GetPlayKey(size_t kind);
}