package simulator.model;

import java.util.Collections;
import java.util.List;
import java.util.ArrayList;

import org.json.JSONObject;

import simulator.factories.Factory;

public class Simulator implements JSONable {
	private Factory<Animal> animalsFactory;
	private Factory<Region> regionsFactory;
	private RegionManager regionManager;
	private List<Animal> animals;
	private double time;

	public Simulator(int cols, int rows, int width, int height, Factory<Animal> animals_factory,
			Factory<Region> regions_factory) {// constructora
		this.animalsFactory = animals_factory;
		this.regionsFactory = regions_factory;
		this.regionManager = new RegionManager(cols, rows, width, height);
		this.animals = new ArrayList<>();
		this.time = 0.0;
	}

	private void set_region(int row, int col, Region r) {
		this.regionManager.set_region(row, col, r);
	}

	public void set_region(int row, int col, JSONObject r_json) {// Creamos la region del JSON
		Region r = regionsFactory.create_instance(r_json);
		set_region(row, col, r);
	}

	private void add_animal(Animal a) {
		this.animals.add(a);
		this.regionManager.register_animal(a);
	}

	public void add_animal(JSONObject a_json) {// añadimos el animal que nos pasan
		Animal a = animalsFactory.create_instance(a_json);
		add_animal(a);
	}

	public MapInfo get_map_info() {
		return this.regionManager;
	}

	public List<? extends AnimalInfo> get_animals() {// devuelve una lista no modificable de los animales
		return Collections.unmodifiableList(animals);
	}

	public double get_time() {
		return this.time;
	}

	public void advance(double dt) {// metodo para actualizar
		this.time += dt;

		ArrayList<Animal> muertos = new ArrayList<Animal>();// creo una listaauxiliar
		for (int i = 0; i < animals.size(); i++) {// recorro mi lista de animales para encontrar a los muertos
			if (animals.get(i).get_state() == State.DEAD) {
				muertos.add(animals.get(i));
				this.regionManager.unregister_animal(animals.get(i));
			}
		}
		// borro a los muertos
		animals.removeAll(muertos);

		for (int i = 0; i < animals.size(); i++) {// actualizo animales
			animals.get(i).update(dt);
			this.regionManager.update_animal_region(animals.get(i));
		}
		// actualizo todas las regiones
		this.regionManager.update_all_regions(dt);

		for (int i = 0; i < animals.size(); i++) {// añado a los bebes de aquellos animales que esten embarazados
			if (animals.get(i).is_pregnant()) {
				this.add_animal(animals.get(i).deliver_baby());
			}
		}//crear lista auxiliar
	}

	public JSONObject as_JSON() {
		JSONObject json = new JSONObject();
		json.put("time", time);
		json.put("state", regionManager.as_JSON());
		return json;
	}
}
