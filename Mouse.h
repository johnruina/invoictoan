#pragma once

#include <bitset>
#include <queue>

class Mouse {
	friend class Window;
public:

	//EVENT CLASS


public:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator = (const Mouse&) = delete;
	
	//I <3 HARDCODING

	std::pair<unsigned int, unsigned int> GetPos() {
		return {X,Y};
	};
	unsigned int GetX() const noexcept {
		return X;
	}
	unsigned int GetY() const noexcept {
		return Y;
	}
	bool MouseIsInWindow() const noexcept {
		return IsInWindow;
	};
	bool IsLeftDown() const noexcept {
		return LeftIsDown;
	};
	bool IsRightDown() const noexcept {
		return RightIsDown;
	};

private:
	//yummy
	void OnMouseMove(unsigned int x, unsigned int y) noexcept;
	void MouseEnter() noexcept;
	void MouseExit() noexcept;
	void RightDown() noexcept;
	void RightUp() noexcept;
	void LeftDown() noexcept;
	void LeftUp() noexcept;

	bool RightIsDown = false;
	bool LeftIsDown = false;
	unsigned int X = 0;
	unsigned int Y = 0;
	bool IsInWindow = false;

};