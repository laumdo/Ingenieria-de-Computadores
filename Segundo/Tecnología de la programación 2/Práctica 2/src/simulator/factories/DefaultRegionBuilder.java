package simulator.factories;

import org.json.JSONObject;

import simulator.model.DefaultRegion;
import simulator.model.Region;

public class DefaultRegionBuilder extends Builder<Region> {

	public DefaultRegionBuilder() {
		super("default", "Infinite food supply");
	}

	@Override
	protected Region create_instance(JSONObject data) {
		DefaultRegion df = new DefaultRegion();
		return df;
	}

	protected void fill_in_data(JSONObject o) {
		
	}
}
