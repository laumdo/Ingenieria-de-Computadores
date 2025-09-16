package simulator.view;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;

import javax.swing.BoxLayout;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JToolBar;
import javax.swing.JTable;

import simulator.control.Controller;

public class MainWindow extends JFrame {
	private Controller _ctrl;

	public MainWindow(Controller ctrl) {
		super("[ECOSYSTEM SIMULATOR]");
		this._ctrl = ctrl;
		initGUI();
	}

	private void initGUI() {
		JPanel mainPanel = new JPanel(new BorderLayout());
		setContentPane(mainPanel);

		// TODO crear ControlPanel y añadirlo en PAGE_START de mainPanel
		JPanel ControlPanel = new ControlPanel(_ctrl); // o JPanel ControlPanel = new ControlPanel(_ctrl);
		mainPanel.add(ControlPanel, BorderLayout.PAGE_START);

		// TODO crear StatusBar y añadirlo en PAGE_END de mainPanel
		JPanel StatusBar = new StatusBar(_ctrl); // JPanel StatusBar = new StatusBar(_ctrl);
		mainPanel.add(StatusBar, BorderLayout.PAGE_END);

		// Definición del panel de tablas (usa un BoxLayout vertical)
		JPanel contentPanel = new JPanel();
		contentPanel.setLayout(new BoxLayout(contentPanel, BoxLayout.Y_AXIS));
		mainPanel.add(contentPanel, BorderLayout.CENTER);

		// Crear la tabla de especies y añadirla a contentPanel.
		// Usa setPreferredSize(new Dimension(500, 250)) para fijar su tamaño
		InfoTable t_animales = new InfoTable("Species", new SpeciesTableModel(_ctrl));
		t_animales.setPreferredSize(new Dimension(500, 250));
		contentPanel.add(t_animales);

		// Crear la tabla de regiones.
		InfoTable t_regiones = new InfoTable("Regions", new RegionsTableModel(_ctrl));
		t_regiones.setPreferredSize(new Dimension(500, 250));
		contentPanel.add(t_regiones);

		// TODO llama a ViewUtils.quit(MainWindow.this) en el método windowClosing
		addWindowListener(new WindowListener() {

			@Override
			public void windowOpened(WindowEvent e) {
				// TODO Auto-generated method stub

			}

			@Override
			public void windowClosing(WindowEvent e) {
				// TODO Auto-generated method stub
				ViewUtils.quit(MainWindow.this);
			}

			@Override
			public void windowClosed(WindowEvent e) {
				// TODO Auto-generated method stub

			}

			@Override
			public void windowIconified(WindowEvent e) {
				// TODO Auto-generated method stub

			}

			@Override
			public void windowDeiconified(WindowEvent e) {
				// TODO Auto-generated method stub

			}

			@Override
			public void windowActivated(WindowEvent e) {
				// TODO Auto-generated method stub

			}

			@Override
			public void windowDeactivated(WindowEvent e) {
				// TODO Auto-generated method stub

			}

		});
		setDefaultCloseOperation(DO_NOTHING_ON_CLOSE);
		pack();
		setVisible(true);
	}

}
