package simulator.model;

import java.util.List;

public class SelectClosest implements SelectionStrategy {

	@Override
	public Animal select(Animal a, List<Animal> as) {
		if (as.isEmpty())
			return null;

		Animal closest = as.get(0);
		double minDistancia = a.get_position().distanceTo(closest.get_position());
		for (Animal animal : as) {
			double distancia = a.get_position().distanceTo(animal.get_position());
			if (distancia < minDistancia) {
				minDistancia = distancia;
				closest = animal;
			}

		}
		return closest;
	}

}
