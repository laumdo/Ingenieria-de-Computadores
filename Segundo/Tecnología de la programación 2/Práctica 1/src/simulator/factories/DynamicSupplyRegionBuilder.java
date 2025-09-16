package simulator.factories;

import org.json.JSONObject;

import simulator.model.DynamicSupplyRegion;
import simulator.model.Region;

public class DynamicSupplyRegionBuilder extends Builder<Region> {

	public DynamicSupplyRegionBuilder() {
		super("dynamic", "Dynamic Supply Region");
	}

	@Override
	protected Region create_instance(JSONObject data) {
		if (data == null) {// data no puede ser nula
			throw new IllegalArgumentException("Data can't be null");
		}
		// pongo los parametros por defecto
		Double food = 1000.0;
		Double factor = 2.0;
		// si nos los dan los modifico
		if (data.has("factor")) {
			factor = data.getDouble("factor");
		}

		if (data.has("food")) {
			food = data.getDouble("food");
		}

		DynamicSupplyRegion dsr = new DynamicSupplyRegion(food, factor);
		return dsr;
	}

	protected void fill_in_data(JSONObject o) {
		Double factor = 2.5;
		o.put("factor", factor);
		Double food = 1250.0;
		o.put("food", food);

	}

}
