
#ifndef OBJECT3DLIST_CLASS_HPP 
# define OBJECT3DLIST_CLASS_HPP 

# include <Object3D.hpp>
# include <memory>
# include <vector>
# include <Interval.hpp>

using std::make_shared;
using std::shared_ptr;

class Object3DList: public Object3D {
public:
	std::vector<shared_ptr<Object3D>> objects;

	Object3DList();
	Object3DList(shared_ptr<Object3D> object);

	void clear();
	void add(shared_ptr<Object3D> object);

	bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override;
};

#endif /* SPHERE_CLASS_HPP */
