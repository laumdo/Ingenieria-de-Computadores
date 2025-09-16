package simulator.control;

import java.io.OutputStream;
import java.util.List;
import java.util.ArrayList;

import org.json.JSONObject;
import org.json.JSONArray;

import simulator.model.Simulator;
import simulator.view.SimpleObjectViewer;
import simulator.view.SimpleObjectViewer.ObjInfo;
import simulator.model.AnimalInfo;
import simulator.model.EcoSysObserver;
import simulator.model.MapInfo;

public class Controller {
	private Simulator _sim;

	public Controller(Simulator sim) {// Constructora
		this._sim = sim;
	}

	public void load_data(JSONObject data) {// Carga las regiones y los animales que le entran
		if (data.has("regions")) {// obtenemos las regiones
			JSONArray regions = data.getJSONArray("regions");
			// recorremos el JSON
			for (int i = 0; i < regions.length(); i++) {//
				this.set_regions(regions.getJSONObject(i));
			}
		}

		if (!data.has("animals")) {// Siempre me tienen que dar los animales, si no los tengo lanzo excepcion
			throw new IllegalArgumentException("animals can't be null");
		}

		JSONArray animals = (JSONArray) data.get("animals");// Obtenemos los animales
		// recorro el JSON de animales
		for (Object animalObj : animals) {
			JSONObject animalJSON = (JSONObject) animalObj;
			int amount = animalJSON.getInt("amount");// obtenemos la cantidad
			if (amount < 0) {// no puede ser negativa
				throw new IllegalArgumentException("amount must be positive");
			}
			JSONObject spec = (JSONObject) animalJSON.get("spec");
			// añadimos a simulator a todos los animales
			for (int i = 0; i < amount; i++) {
				_sim.add_animal(spec);
			}
		}
	}

	public void run(double t, double dt, boolean sv, OutputStream out) {
		SimpleObjectViewer view = null;
		if (sv) {// si tenemos el visor
			MapInfo m = _sim.get_map_info();
			view = new SimpleObjectViewer("[ECOSYSTEM]", m.get_width(), m.get_height(), m.get_cols(), m.get_rows());
			view.update(to_animals_info(_sim.get_animals()), _sim.get_time(), dt);
		}

		JSONObject init_state = _sim.as_JSON();
		// mientras que el tiempo sea inferior al que nos han dado ejecutamos
		while (_sim.get_time() <= t) {
			_sim.advance(dt);
			if (sv)
				view.update(to_animals_info(_sim.get_animals()), _sim.get_time(), dt);
		}
		JSONObject final_state = _sim.as_JSON();

		JSONObject result = new JSONObject();
		result.put("in", init_state);
		result.put("out", final_state);
		if (sv)
			view.close();

	}

	private List<ObjInfo> to_animals_info(List<? extends AnimalInfo> animals) {
		List<ObjInfo> ol = new ArrayList<>(animals.size());
		for (AnimalInfo a : animals)
			ol.add(new ObjInfo(a.get_genetic_code(), (int) a.get_position().getX(), (int) a.get_position().getY(),
					(int) Math.round(a.get_age()) + 2));
		return ol;
	}

	public void reset(int cols, int rows, int width, int height) {
		this._sim.reset(cols, rows, width, height);
	}

	public void set_regions(JSONObject rs) {
		JSONArray row = rs.getJSONArray("row");// obtengo la fila
		JSONArray col = rs.getJSONArray("col");// obtengo la columna
		int rf = row.getInt(0);
		int rt = row.getInt(1);
		int cf = col.getInt(0);
		int ct = col.getInt(1);
		JSONObject spec = rs.getJSONObject("spec");
		// recorro las filas y las columnas para crear las regiones en la clase
		// simulator
		for (int r = rf; r <= rt; r++) {
			for (int c = cf; c <= ct; c++) {
				_sim.set_region(r, c, spec);
			}
		}
	}

	public void advance(double dt) {
		this._sim.advance(dt);
	}

	public void addObserver(EcoSysObserver o) {
		this._sim.addObserver(o);
	}

	public void removeObserver(EcoSysObserver o) {
		this._sim.removeObserver(o);
	}
}
