package simulator.view;

import java.util.List;
import java.util.Map;
import java.util.HashMap;
import java.util.Iterator;
import java.util.ArrayList;

import javax.swing.table.AbstractTableModel;

import simulator.control.Controller;
import simulator.model.AnimalInfo;

import simulator.model.EcoSysObserver;
import simulator.model.MapInfo;
import simulator.model.RegionInfo;
import simulator.model.State;

public class SpeciesTableModel extends AbstractTableModel implements EcoSysObserver {
	List<String> _header = new ArrayList<>();
	Map<String, Map<State, Integer>> data;
	List<String> animalsName;

	SpeciesTableModel(Controller ctrl) {
		// inicializar estructuras de datos correspondientes
		this.data = new HashMap<>();
		this.animalsName = new ArrayList<>();
		this._header.add("Species");
		for (State s : State.values()) {
			this._header.add(s.toString());
		}
		// Registrar this como observador
		ctrl.addObserver(this);
	}

	@Override
	public int getRowCount() {
		return this.data.size();
	}

	@Override
	public String getColumnName(int i) {
		return _header.get(i);
	}

	@Override
	public int getColumnCount() {
		return this._header.size();
	}

	@Override
	public Object getValueAt(int rowIndex, int columnIndex) {
		Object obj = null;
		String name = animalsName.get(rowIndex);
		Map<State, Integer> a = this.data.get(name);
		switch (columnIndex) {
		case 0:
			obj = name; // Obtiene la primera clave del mapa (el c�digo gen�tico)
			break;
		default:
			State s = State.values()[columnIndex - 1];
			// obj = a.getOrDefault(s, 0);
			obj = a.get(s);
			break;
		}

		return obj;
	}

	@Override
	public void onRegister(double time, MapInfo map, List<AnimalInfo> animals) {
		data.clear();
		updateData(animals);
		fireTableDataChanged();
	}

	@Override
	public void onReset(double time, MapInfo map, List<AnimalInfo> animals) {
		this.data.clear();
		System.out.println(animals);
		updateData(animals);
		System.out.println(data);
		fireTableStructureChanged();
	}

	@Override
	public void onAnimalAdded(double time, MapInfo map, List<AnimalInfo> animals, AnimalInfo a) {
		System.out.println("ESTOY EN ON ANIMAL ADDED");
		System.out.println(data);
		System.out.println(animalsName);
		Map<State, Integer> b = new HashMap<>();
		int cont = 0;
		
		if(!animalsName.contains(a.get_genetic_code())){
			this.animalsName.add(a.get_genetic_code());
		}
		
		if (data != null) {
			b = data.get(a.get_genetic_code());
			if (b != null) {
				cont = b.get(a.get_state());
			}
		}
		if (b == null) {
			b = new HashMap<>();
		}
		b.put(a.get_state(), cont+1);
		data.put(a.get_genetic_code(), b);
		fireTableDataChanged();
	}

	@Override
	public void onRegionSet(int row, int col, MapInfo map, RegionInfo r) {
		// No es necesario para esta tabla

	}

	@Override
	public void onAvanced(double time, MapInfo map, List<AnimalInfo> animals, double dt) {
		data.clear();
		updateData(animals);
		fireTableDataChanged();
		// No es necesario para esta tabla
	}

	private void updateData(List<AnimalInfo> animals) {
		Iterator<AnimalInfo> a = animals.iterator();
		Map<State, Integer> c;
		int cont;
		AnimalInfo aNext;
		animalsName.clear();
		while (a.hasNext()) {
			aNext = a.next();
			if (!animalsName.contains(aNext.get_genetic_code())) {
				Map<State, Integer> b = new HashMap<State, Integer>();
				for (State s : State.values()) {
					b.put(s, 0);
				}
				animalsName.add(aNext.get_genetic_code());
				data.put(aNext.get_genetic_code(), b);
			}
			c = new HashMap<State, Integer>(data.get(aNext.get_genetic_code()));
			cont = c.get(aNext.get_state()) + 1;
			c.put(aNext.get_state(), cont);
			data.put(aNext.get_genetic_code(), c);

		}
	}
}
