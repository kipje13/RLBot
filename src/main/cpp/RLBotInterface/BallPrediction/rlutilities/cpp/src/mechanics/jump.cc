#include "mechanics/jump.h"

const float Jump::speed = 291.667f;
const float Jump::acceleration = 1458.3333f;
const float Jump::min_duration = 0.025f;
const float Jump::max_duration = 0.2f;

Jump::Jump(Car & c) : car(c) {
	duration = 0.0f;
	finished = false;
	controls = Input();

	timer = 0.0f;
	counter = 0;
}

void Jump::step(float dt) {

	controls.jump = (timer < fmaxf(duration, min_duration)) ? 1 : 0;

	finished = (controls.jump == 0);

	timer += dt;

}

Car Jump::simulate() {

	// make a copy of the car's state and get a pointer to it
	Car car_copy = Car(car);
	Jump copy = Jump(car_copy);
	copy.duration = duration;

	float dt = 0.01666f;
	for (float t = dt; t < Jump::max_duration; t += dt) {

		// get the new controls
		copy.step(dt);

		// and simulate their effect on the car
		car_copy.step(copy.controls, dt);

		if (copy.finished) break;
	}

	return car_copy;
}

#ifdef GENERATE_PYTHON_BINDINGS
#include <pybind11/pybind11.h>
void init_jump(pybind11::module & m) {
	pybind11::class_<Jump>(m, "Jump")
		.def(pybind11::init<Car &>())
		.def_readwrite("duration", &Jump::duration)
		.def_readwrite("finished", &Jump::finished)
		.def_readwrite("controls", &Jump::controls)
		.def_readonly_static("speed", &Jump::speed)
		.def_readonly_static("acceleration", &Jump::acceleration)
		.def_readonly_static("min_duration", &Jump::min_duration)
		.def_readonly_static("max_duration", &Jump::max_duration)
		.def("step", &Jump::step)
		.def("simulate", &Jump::simulate);
}
#endif
