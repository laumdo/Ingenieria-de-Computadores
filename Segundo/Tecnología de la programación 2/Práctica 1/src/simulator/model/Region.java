package simulator.model;

import java.util.List;

import org.json.JSONArray;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.Collections;

public abstract class Region implements Entity, FoodSupplier, RegionInfo {
	protected List<Animal> listaAnimales;

	public Region() {
		listaAnimales = new ArrayList<>();
	}

	final void add_animal(Animal a) {// añadimos el animal a la lista de la region
		listaAnimales.add(a);
	}

	final void remove_animal(Animal a) {// quitamos el animal de la lista de la region
		listaAnimales.remove(a);
	}

	final List<Animal> getAnimals() {// devuelve una lista no modificable de todos los animales de la region
		return Collections.unmodifiableList(listaAnimales);
	}

	public JSONObject as_JSON() {
		JSONObject j = new JSONObject();
		JSONArray arr = new JSONArray();
		for (int i = 0; i < listaAnimales.size(); i++) {
			arr.put(i, listaAnimales.get(i).as_JSON());
		}
		j.put("animals", arr);
		return j;
	}
}
