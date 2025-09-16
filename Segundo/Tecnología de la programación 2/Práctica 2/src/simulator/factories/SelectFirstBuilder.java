package simulator.factories;

import org.json.JSONObject;

import simulator.model.SelectFirst;
import simulator.model.SelectionStrategy;

public class SelectFirstBuilder extends Builder<SelectionStrategy> {

	public SelectFirstBuilder() {
		super("first", "Select First");
	}

	@Override
	protected SelectFirst create_instance(JSONObject data) {
		SelectFirst sf = new SelectFirst();
		return sf;
	}

}