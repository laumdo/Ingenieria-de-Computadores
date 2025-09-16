package simulator.factories;

import java.util.Collections;
import java.util.List;
import java.util.ArrayList;
import java.util.Map;
import java.util.HashMap;

import org.json.JSONObject;

public class BuilderBasedFactory<T> implements Factory<T> {
	private Map<String, Builder<T>> _builders;
	private List<JSONObject> _builders_info;

	public BuilderBasedFactory() {// constructora
		// Creamos un mapa y una lista
		this._builders = new HashMap<>();
		this._builders_info = new ArrayList<>();
	}

	public BuilderBasedFactory(List<Builder<T>> builders) {
		this();
		// llamamos a add_builderpara cada builder

		for (Builder<T> b : builders) {
			this.add_builder(b);
		}
	}

	public void add_builder(Builder<T> b) {
		// añadimos el typetag y la info
		_builders.put(b.get_type_tag(), b);
		_builders_info.add(b.get_info());
	}

	@Override
	public T create_instance(JSONObject info) {
		if (info == null) {
			throw new IllegalArgumentException("’info’ cannot be null");
		}
		T obj = null;
		if (_builders.containsKey(info.getString("type"))) {
			String clave = info.getString("type");
			obj = _builders.get(clave)
					.create_instance(info.has("data") ? info.getJSONObject("data") : new JSONObject());
		}

		if (obj == null) {
			throw new IllegalArgumentException("Unrecognized ‘info’:" + info.toString());
		} else {
			return obj;
		}
	}

	@Override
	public List<JSONObject> get_info() {
		return Collections.unmodifiableList(_builders_info);
	}

}
