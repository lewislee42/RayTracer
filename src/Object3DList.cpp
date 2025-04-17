# include <Object3DList.hpp>

Object3DList::Object3DList() {

}

Object3DList::Object3DList(shared_ptr<Object3D> object) {
	add(object);
}

void Object3DList::clear() {
	this->objects.clear();
}

void Object3DList::add(shared_ptr<Object3D> object) {
	this->objects.push_back(object);
}

bool Object3DList::hit(const Ray& r, Interval rayT, HitRecord& rec) const {
	HitRecord	tempRec;
	bool		hitAnything = false;
	double		closestSoFar = rayT.max;

	for (const shared_ptr<Object3D>& object: this->objects) {
		if (object->hit(r, Interval(rayT.min, closestSoFar), tempRec)) {
			hitAnything = true;
			closestSoFar = tempRec.t;
			rec = tempRec;
		}
	}

	return hitAnything;
}
