package simulator.view;

import java.awt.Dimension;
import java.awt.FlowLayout;
import java.util.List;

import javax.swing.BorderFactory;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSeparator;

import simulator.control.Controller;
import simulator.model.AnimalInfo;
import simulator.model.EcoSysObserver;
import simulator.model.MapInfo;
import simulator.model.RegionInfo;

public class StatusBar extends JPanel implements EcoSysObserver {
	// TODO Añadir los atributos necesarios.
	private double time = 0.0;
	private int animales;
	private int anchura;
	private int altura;
	private int filas;
	private int columnas;

	JLabel t;
	JLabel a;
	JLabel dimension;

	StatusBar(Controller ctrl) {
		initGUI();
		// TODO registrar this como observador
		ctrl.addObserver(this);
	}

	private void initGUI() {
		this.setLayout(new FlowLayout(FlowLayout.LEFT));
		this.setBorder(BorderFactory.createBevelBorder(1));
		// TODO Crear varios JLabel para el tiempo, el número de animales, y la
		// dimensión y añadirlos al panel. Puedes utilizar el siguiente código
		// para añadir un separador vertical:

		t = new JLabel("Time: " + time);
		this.add(t);
		JSeparator s = new JSeparator(JSeparator.VERTICAL);
		s.setPreferredSize(new Dimension(10, 20));
		this.add(s);

		a = new JLabel("Total Animals: " + animales);
		this.add(a);
		JSeparator s2 = new JSeparator(JSeparator.VERTICAL);
		s2.setPreferredSize(new Dimension(10, 20));
		this.add(s2);

		dimension = new JLabel("Dimension: " + anchura + "x" + altura + " " + filas + "x" + columnas);
		this.add(dimension);
	}
	// TODO el resto de métodos van aquí…

	@Override
	public void onRegister(double time, MapInfo map, List<AnimalInfo> animals) {
		updateStatusBar(time, animals.size(), map.get_width(), map.get_height(), map.get_rows(), map.get_cols());

	}

	@Override
	public void onReset(double time, MapInfo map, List<AnimalInfo> animals) {
		this.time = 0.0;
		this.animales = animals.size();
	}

	@Override
	public void onAnimalAdded(double time, MapInfo map, List<AnimalInfo> animals, AnimalInfo a) {
		updateStatusBar(time, animals.size(), map.get_width(), map.get_height(), map.get_rows(), map.get_cols());

	}

	@Override
	public void onRegionSet(int row, int col, MapInfo map, RegionInfo r) {
		// No es necesario actualizar la barra de estado cuando se establece una región

	}

	@Override
	public void onAvanced(double time, MapInfo map, List<AnimalInfo> animals, double dt) {
		updateStatusBar(time, animals.size(), map.get_width(), map.get_height(), map.get_rows(), map.get_cols());
	}

	private void updateStatusBar(double time, int totalAnimals, int width, int height, int rows, int columns) {
		this.time = time;
		this.animales = totalAnimals;
		this.anchura = width;
		this.altura = height;
		this.filas = rows;
		this.columnas = columns;

		t.setText("Time: " + time);
		a.setText("Total Animals: " + totalAnimals);
		dimension.setText("Dimension: " + width + "x" + height + " " + rows + "x" + columns);
	}
}
