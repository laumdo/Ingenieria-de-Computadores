package simulator.model;

import java.util.List;
import java.util.Map;
import java.util.function.Predicate;
import java.util.ArrayList;
import java.util.HashMap;

import simulator.misc.Utils;

import org.json.JSONArray;
import org.json.JSONObject;

public class RegionManager implements AnimalMapView {

	private int width;
	private int height;
	private int cols;
	private int rows;
	private int region_width;
	private int region_height;
	private Region[][] _regions;
	private Map<Animal, Region> _animal_region;

	public RegionManager(int cols, int rows, int width, int height) {// constructora
		this.cols = cols;
		this.rows = rows;
		this.width = width;
		this.height = height;
		this.region_width = this.width / this.cols;
		this.region_height = this.height / this.rows;
		this._regions = new Region[this.rows][this.cols];
		for (int i = 0; i < this.rows; i++) {
			for (int j = 0; j < this.cols; j++) {
				_regions[i][j] = new DefaultRegion();
			}
		}
		this._animal_region = new HashMap<Animal, Region>();

	}

	// getters
	@Override
	public int get_cols() {
		return this.cols;
	}

	@Override
	public int get_rows() {
		return this.rows;
	}

	@Override
	public int get_width() {
		return this.width;
	}

	@Override
	public int get_height() {
		return this.height;
	}

	@Override
	public int get_region_width() {
		return this.region_width;
	}

	@Override
	public int get_region_height() {
		return this.region_height;
	}

	// metodos
	@Override
	public double get_food(Animal a, double dt) {// metodo que devuelve la comida de una region
		int row = (int) a.get_position().getY() / region_height;
		int col = (int) a.get_position().getX() / region_width;

		return _regions[row][col].get_food(a, dt);
	}

	void set_region(int row, int col, Region r) {// metodo para actualizar la region que me pasan por parametro
		for (Animal a : this._regions[row][col].getAnimals()) {
			r.add_animal(a);
			this._animal_region.put(a, r);
		}
		this._regions[row][col] = r;
	}

	void register_animal(Animal a) {// metodo para añadir un animal a su region
		a.init(this);
		int row = (int) a.get_position().getY() / region_height;
		int col = (int) a.get_position().getX() / region_width;
		this._regions[row][col].add_animal(a);
		this._animal_region.put(a, this._regions[row][col]);
	}

	void unregister_animal(Animal a) {// metodo para eliminar un animal de su region
		int row = (int) a.get_position().getY() / region_height;
		int col = (int) a.get_position().getX() / region_width;
		this._regions[row][col].remove_animal(a);
		this._animal_region.remove(a);
	}

	void update_animal_region(Animal a) {// modificamos la region a la que pertenece un animal en caso de que haya
											// cambiado
		int row = (int) a.get_position().getY() / region_height;
		int col = (int) a.get_position().getX() / region_width;
		if (this._regions[row][col] != this._animal_region.get(a)) {
			this._animal_region.get(a).remove_animal(a);
			this._regions[row][col].add_animal(a);
			this._animal_region.put(a, this._regions[row][col]);
		}
	}

	void update_all_regions(double dt) {// actualizo todas las regiones
		for (int i = 0; i < this.cols; i++) {
			for (int j = 0; j < this.rows; j++) {
				_regions[j][i].update(dt);
			}
		}
	}

	@Override
	public List<Animal> get_animals_in_range(Animal e, Predicate<Animal> filter) {// metodo para encontrar los animales
																					// que cumplan una condicion dentro
																					// de un rango
		int y1 = (int) ((e.get_position().getY() - e.get_sight_range()) / region_height);
		int x1 = (int) ((e.get_position().getX() - e.get_sight_range()) / region_width);
		int y2 = (int) ((e.get_position().getY() + e.get_sight_range()) / region_height);
		int x2 = (int) ((e.get_position().getX() + e.get_sight_range()) / region_width);

		y1 = (int) Utils.constrain_value_in_range(y1, 0, rows - 1);
		x1 = (int) Utils.constrain_value_in_range(x1, 0, cols - 1);
		y2 = (int) Utils.constrain_value_in_range(y2, 0, rows - 1);
		x2 = (int) Utils.constrain_value_in_range(x2, 0, cols - 1);

		List<Animal> listaAux = new ArrayList<>();
		// recorro las regiones dentro del rango
		for (int i = x1; i <= x2; i++) {
			for (int j = y1; j <= y2; j++) {
				List<Animal> listait = this._regions[j][i].getAnimals();
				for (Animal a : listait) {
					// si esta dentro del rango y cumple la condicion añado el animal a mi lista
					// auxiliar
					if (!a.equals(e) && (a.get_position().distanceTo(e.get_position()) < e.get_sight_range())
							&& filter.test(a)) {
						listaAux.add(a);
					}
				}
			}
		}
		return listaAux;
	}

	public JSONObject as_JSON() {
		JSONObject obj2 = new JSONObject();
		JSONArray arr = new JSONArray();
		int cont = 0;
		for (int i = 0; i < this.cols; i++) {
			for (int j = 0; j < this.rows; j++) {
				JSONObject obj1 = new JSONObject();
				obj1.put("row", j);
				obj1.put("col", i);
				obj1.put("data", this._regions[j][i]);
				arr.put(cont, obj1);
				cont++;
			}
		}
		obj2.put("regiones", arr);
		return obj2;
	}

}
