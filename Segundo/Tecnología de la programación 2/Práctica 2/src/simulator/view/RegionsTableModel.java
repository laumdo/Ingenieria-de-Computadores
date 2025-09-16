package simulator.view;

import java.util.List;
import java.util.Map;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;

import javax.swing.table.AbstractTableModel;

import simulator.control.Controller;
import simulator.model.Animal;
import simulator.model.AnimalInfo;
import simulator.model.AnimalMapView;
import simulator.model.Diet;
import simulator.model.EcoSysObserver;
import simulator.model.MapInfo;
import simulator.model.Region;
import simulator.model.RegionInfo;
import simulator.model.RegionManager;
import simulator.model.MapInfo.RegionData;

public class RegionsTableModel extends AbstractTableModel implements EcoSysObserver {
	//String[] _header= {"Row", "Col", "Desc."/*, "Herbivores", "Carnivores"*/};
	List<String> _header = new ArrayList<>();
	//List<Region> _regions;
	List<RegionData> data;
	Map<RegionData,Map<Diet,Integer>> misregiones;
	
	RegionsTableModel(Controller ctrl) {
		this._header.add("Row");
		this._header.add("Col");
		this._header.add("Desc");
	//Inicializar estructuras de datos correspondientes
		this.data=new ArrayList<>();
		for(Diet d: Diet.values()) {
			this._header.add(d.toString());
		}
		misregiones = new HashMap<MapInfo.RegionData, Map<Diet,Integer>>();
	//Registrar this como observador
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
		List<AnimalInfo> a = data.get(rowIndex).r().getAnimalsInfo();
		  // Obtener informaci�n de la regi�n en la fila actual
        //Region region = regionData.getRegion();
        switch (columnIndex) {
            case 0:
            	obj=data.get(rowIndex).row();
                //return regionData.getRegion(); // Fila de la regi�n
            	break;
            case 1:
            	obj=data.get(rowIndex).col();
                //return regionData.getCol(); // Columna de la regi�n
            	break;
            case 2:
            	obj=data.get(rowIndex).r();
            	break;
            default:
                // N�mero de animales en la regi�n para el estado correspondiente a la columna actual
            	
	                Diet diet = Diet.values()[columnIndex - 3];
	                /*
	                RegionData d = data.get(rowIndex);
	                obj = misregiones.get(d).get(diet);
	                
	                */
	                int cont=0;
	                for (AnimalInfo b: a) {
	                	if(b.get_diet().equals(diet)) {
	                		cont++;
	                	}
	                }
	                obj = cont;
            	
                break;
        }
        return obj;
	}

	@Override
	public void onRegister(double time, MapInfo map, List<AnimalInfo> animals) {
		Iterator<RegionData> a = map.iterator();
		while(a.hasNext()) {
			RegionData reg = a.next();
			data.add(reg);
			
			Map<Diet,Integer> mapadietas = new HashMap<Diet, Integer>();
			for(Diet d: Diet.values()){
				mapadietas.put(d,0);
			}
			
			for(AnimalInfo animal: reg.r().getAnimalsInfo()) {
				Diet da= animal.get_diet();
				mapadietas.put(da,mapadietas.getOrDefault(da,0)+1);
			}
			
			misregiones.put(reg, mapadietas);
			
			
			
		}
		/*for(String i:data.keySet())
			_groups.add(groups.get(i));
		*/
		//for(BodiesGroup g: groups.values()){
			//_groups.add(g);
		//}
		fireTableStructureChanged();
	}

	@Override
	public void onReset(double time, MapInfo map, List<AnimalInfo> animals) {
		this.data.clear();
		fireTableStructureChanged();
	}

	@Override
	public void onAnimalAdded(double time, MapInfo map, List<AnimalInfo> animals, AnimalInfo a) {
		this.data.clear();
		Iterator<RegionData> m = map.iterator();
		while(m.hasNext()) {
			data.add(m.next());
			
		}
		fireTableStructureChanged();
		
	}

	@Override
	public void onRegionSet(int row, int col, MapInfo map, RegionInfo r) {
		// TODO Auto-generated method stub
		this.data.clear();
		Iterator<RegionData> m = map.iterator();
		while(m.hasNext()) {
			data.add(m.next());
			
		}
		fireTableStructureChanged();
	}

	@Override
	public void onAvanced(double time, MapInfo map, List<AnimalInfo> animals, double dt) {
		// TODO Auto-generated method stub
		this.data.clear();
		Iterator<RegionData> m = map.iterator();
		while(m.hasNext()) {
			data.add(m.next());
			
		}
		fireTableStructureChanged();
	}
	
	// M�todo privado para actualizar los datos de la tabla
    /*private void updateData(MapInfo map) {
        // Obtener la informaci�n de las regiones del mapa
        RegionInfo[][] regions = map.getRegions();

        // Actualizar el tama�o de los datos
        data = new Object[regions.length * regions[0].length][columnNames.length];

        // Rellenar los datos con la informaci�n de las regiones
        int index = 0;
        for (int i = 0; i < regions.length; i++) {
            for (int j = 0; j < regions[0].length; j++) {
                RegionInfo region = regions[i][j];
                data[index][0] = i; // Fila
                data[index][1] = j; // Columna
                data[index][2] = region.getDescription(); // Descripci�n de la regi�n
                // Contar el n�mero de animales para cada tipo de dieta
                Map<Diet, Integer> dietCounts = region.getDietCounts();
                data[index][3] = dietCounts.getOrDefault(Diet.HERBIVORE, 0); // Herb�voros
                data[index][4] = dietCounts.getOrDefault(Diet.CARNIVORE, 0); // Carn�voros
                index++;
            }
        }

        // Notificar a los observadores que los datos han cambiado
        fireTableDataChanged();
    }*/
}
