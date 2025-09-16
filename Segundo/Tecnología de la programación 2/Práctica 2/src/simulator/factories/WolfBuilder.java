package simulator.factories;

import org.json.JSONArray;
import org.json.JSONObject;

import simulator.misc.Utils;
import simulator.misc.Vector2D;
import simulator.model.Animal;
import simulator.model.SelectFirst;
import simulator.model.SelectionStrategy;
import simulator.model.Wolf;

public class WolfBuilder extends Builder<Animal> {

	private Factory<SelectionStrategy> f;

	public WolfBuilder(Factory<SelectionStrategy> f) {
		super("wolf", "Wolf");
		this.f = f;
	}

	@Override
	protected Wolf create_instance(JSONObject data) {
		if (data == null) {
			throw new IllegalArgumentException("Data can't be null");
		}
		// Creo mate_strategy, le doy el valor por defecto, y si me lo dan lo modifico
		SelectionStrategy mate_strategy = new SelectFirst();
		if (data.has("mate_strategy")) {
			JSONObject m = data.getJSONObject("mate_strategy");
			mate_strategy = f.create_instance(m);
		}
		// Creo hunt_strategy, le doy el valor por defecto, y si me lo dan lo modifico
		SelectionStrategy hunt_strategy = new SelectFirst();
		if (data.has("hunt_strategy")) {
			JSONObject h = data.getJSONObject("hunt_strategy");
			hunt_strategy = f.create_instance(h);
		}

		Vector2D pos = null;// creo pos
		if (data.has("pos")) {// si data tiene pos actualizo los datos con los que me dan
			JSONArray x_range = data.getJSONObject("pos").getJSONArray("x_range");
			JSONArray y_range = data.getJSONObject("pos").getJSONArray("y_range");
			// creo una posicion aleatoria dentro de los rangos que me dan
			pos = new Vector2D(Utils._rand.nextDouble(x_range.getDouble(0), x_range.getDouble(1)),
					Utils._rand.nextDouble(y_range.getDouble(0), y_range.getDouble(1)));
		}

		Wolf w = new Wolf(mate_strategy, hunt_strategy, pos);
		return w;
	}

	protected void fill_in_data(JSONObject o) {
		o.put("mate_strategy", "{ ... }");
		o.put("hunt_strategy", "{ ... }");
		JSONArray x = new JSONArray();
		x.put(0, 100.0);
		x.put(1, 200.0);
		JSONArray y = new JSONArray();
		y.put(0, 100.0);
		y.put(1, 200.0);
		JSONObject pos = new JSONObject();
		pos.put("x_range", x);
		pos.put("y_range", y);
		o.put("pos", pos);
	}

}