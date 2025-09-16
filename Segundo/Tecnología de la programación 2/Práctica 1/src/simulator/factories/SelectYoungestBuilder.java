package simulator.factories;

import org.json.JSONObject;

import simulator.model.SelectYoungest;
import simulator.model.SelectionStrategy;

public class SelectYoungestBuilder extends Builder<SelectionStrategy> {

	public SelectYoungestBuilder() {
		super("youngest", "SelectYoungest");
	}

	@Override
	protected SelectYoungest create_instance(JSONObject data) {
		SelectYoungest sy = new SelectYoungest();
		return sy;
	}

}
