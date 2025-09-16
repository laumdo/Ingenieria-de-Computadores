package simulator.model;

import java.util.List;

import org.json.JSONArray;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.Collections;

public abstract class Region implements Entity, FoodSupplier, RegionInfo {
	protected List<Animal> _animals;

	public Region() {
		_animals = new ArrayList<>();
	}

	final void add_animal(Animal a) {// añadimos el animal a la lista de la region
		_animals.add(a);
	}

	final void remove_animal(Animal a) {// quitamos el animal de la lista de la region
		_animals.remove(a);
	}

	final List<Animal> getAnimals() {// devuelve una lista no modificable de todos los animales de la region
		return Collections.unmodifiableList(_animals);
	}

	public JSONObject as_JSON() {
		JSONObject j = new JSONObject();
		JSONArray arr = new JSONArray();
		for (int i = 0; i < _animals.size(); i++) {
			arr.put(i, _animals.get(i).as_JSON());
		}
		j.put("animals", arr);
		return j;
	}

	@Override
	public List<AnimalInfo> getAnimalsInfo() {
		return new ArrayList<>(_animals); // se puede usar Collections.unmodifiableList(_animals);
	}
}
