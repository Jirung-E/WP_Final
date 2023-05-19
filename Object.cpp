#include "Object.h"


Object::Object(const Point& position) : position { position }, velocity { 0, 0 }, accelerate { 0, 0 }, volume { 1, 1 },
current_anim_order { 0 } {

}


//void Object::addSprite(const CImage& source) {
//	sprites.push_back(source);
//	animation_order.push_back(sprites.size()-1);
//}

void Object::addSprite(Sprite* source) {
	sprites.push_back(source);
	animation_order.push_back(sprites.size()-1);
}

void Object::setSpriteSizePer(double size_per) {
	for(auto& e : sprites) {
		e->size_per = size_per;
	}
}

void Object::draw(const HDC& hdc, const RECT& valid_area, const Map& map) const {
	RECT area = map.absoluteArea(valid_area);
	int w = area.right - area.left;
	int h = area.bottom - area.top;
	int obj_w = volume.width / map.getWidth() * w;
	int obj_h = volume.height / map.getHeight() * h;
	POINT abs_point = getAbsolutePosition(valid_area, map);
	area = { abs_point.x - obj_w/2, abs_point.y - obj_h/2, abs_point.x + obj_w/2, abs_point.y + obj_h/2 };
	sprites[animation_order[current_anim_order]]->draw(hdc, area);
}

void Object::setAnimationSpriteNext() {
	current_anim_order++;
	if(current_anim_order >= animation_order.size()) {
		current_anim_order = 0;
	}
}

POINT Object::getAbsolutePosition(const RECT& valid_area, const Map& map) const {
	RECT area = map.absoluteArea(valid_area);
	int w = area.right - area.left;
	int h = area.bottom - area.top;
	int x = area.left + position.x / map.getWidth() * w;
	int y = area.top + position.y / map.getHeight() * h;
	return { x, y };
}


void Object::move(const Vector& vector, const Map& map) {
	velocity = vector;
	if(velocity.scalar() > 1) {
		velocity = velocity.unit();
	}
	move(map);
}

void Object::move(const Map& map) {
	position += velocity;

	//if(position.x - volume.width/2 <= 0) {
	//	position.x = volume.width/2;
	//}
	//else if(position.x + volume.width/2 >= map.getWidth()) {
	//	position.x = map.getWidth() - volume.width/2;
	//}
	//if(position.y - volume.height/2 <= 0) {
	//	position.y = volume.height/2;
	//}
	//else if(position.y + volume.height/2 >= map.getHeight()) {
	//	position.y = map.getHeight() - volume.height/2;
	//}
}