#include "MouseEvent.h"

MouseEvent::MouseEvent() : type(EventType::Invalid), x(0), y(0) {

}

MouseEvent::MouseEvent(const EventType type, const int x, const int y) : type(type), x(x), y(y) {

}

bool MouseEvent::IsValid() const {
	return this->type != Invalid;
}

MouseEvent::EventType MouseEvent::GetType() const {
	return this->type;
}

MousePoint MouseEvent::GetPos() const {
	return { this->x,this->y };
};

int MouseEvent::GetPosX() const {
	return x;
}

int MouseEvent::GetPosY() const {
	return y;
}