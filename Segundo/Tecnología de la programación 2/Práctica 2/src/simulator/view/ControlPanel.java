package simulator.view;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.List;
import java.io.FileInputStream;

import javax.swing.Box;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSpinner;
import javax.swing.JTextField;
import javax.swing.JToolBar;
import javax.swing.SpinnerNumberModel;
import javax.swing.SwingUtilities;

import org.json.JSONObject;
import org.json.JSONTokener;

import simulator.control.Controller;
//import simulator.misc.Utils;
import simulator.misc.Utils;
import simulator.model.AnimalInfo;
import simulator.model.EcoSysObserver;
import simulator.model.MapInfo;
import simulator.model.RegionInfo;

public class ControlPanel extends JPanel implements EcoSysObserver, ActionListener {
	private Controller _ctrl;
	private ChangeRegionsDialog _changeRegionsDialog;
	private JToolBar _toolaBar;
	private JFileChooser _fc;
	private boolean _stopped = true; // utilizado en los botones de run/stop
	private JButton _quitButton;
	// TODO a�ade m�s atributos aqu� �
	private JButton _openButton;
	private JButton _viewerButton;
	private JButton _regionsButton;
	private JButton _runButton;
	private JButton _stopButton;

	private JTextField dt;
	private JSpinner steps;

	ControlPanel(Controller ctrl) {
		this._ctrl = ctrl;
		initGUI();
		this._ctrl.addObserver(this);
	}

	private void initGUI() {
		setLayout(new BorderLayout());
		_toolaBar = new JToolBar();
		add(_toolaBar, BorderLayout.PAGE_START);
		// TODO crear los diferentes botones/atributos y a�adirlos a _toolaBar.
		// Todos ellos han de tener su correspondiente tooltip. Puedes utilizar
		// _toolaBar.addSeparator() para a�adir la l�nea de separaci�n vertical
		// entre las componentes que lo necesiten.

		// Open Button
		_toolaBar.addSeparator();
		_openButton = new JButton();
		_openButton.setToolTipText("Open");
		_openButton.setIcon(new ImageIcon("resources/icons/open.png"));
		_openButton.addActionListener(this);
		_toolaBar.add(_openButton);

		// Viewer Button
		_toolaBar.addSeparator();
		_viewerButton = new JButton();
		_viewerButton.setToolTipText("Viewer");
		_viewerButton.setIcon(new ImageIcon("resources/icons/viewer.png"));
		_viewerButton.addActionListener(this);
		_toolaBar.add(_viewerButton);

		// Regions Button
		_toolaBar.addSeparator();
		_regionsButton = new JButton();
		_regionsButton.setToolTipText("Regions");
		_regionsButton.setIcon(new ImageIcon("resources/icons/regions.png"));
		_regionsButton.addActionListener(this);
		_toolaBar.add(_regionsButton);

		// Run Button
		_toolaBar.addSeparator();
		_runButton = new JButton();
		_runButton.setToolTipText("Run");
		_runButton.setIcon(new ImageIcon("resources/icons/run.png"));
		_runButton.addActionListener(this);
		_toolaBar.add(_runButton);

		// Stop Button
		_toolaBar.addSeparator(); // ?
		_stopButton = new JButton();
		_stopButton.setToolTipText("Stop");
		_stopButton.setIcon(new ImageIcon("resources/icons/stop.png"));
		_stopButton.addActionListener(this);
		_toolaBar.add(_stopButton);

		_toolaBar.add(new JLabel("Steps: "));
		steps = new JSpinner(new SpinnerNumberModel(10000, 1, 10000, 100));
		steps.setToolTipText("Steps: 1-10000");
		steps.setPreferredSize(new Dimension(60, 30));
		steps.setMaximumSize(new Dimension(60, 30));
		_toolaBar.add(steps);

		_toolaBar.add(new JLabel("Delta-Time: "));
		dt = new JTextField(5);
		dt.setPreferredSize(new Dimension(60, 30));
		dt.setMaximumSize(new Dimension(60, 30));
		dt.setText("0.03");
		_toolaBar.add(dt);

		// Quit Button
		_toolaBar.add(Box.createGlue()); // this aligns the button to the right
		_toolaBar.addSeparator();
		_quitButton = new JButton();
		_quitButton.setToolTipText("Quit");
		_quitButton.setIcon(new ImageIcon("resources/icons/exit.png"));
		_quitButton.addActionListener((e) -> ViewUtils.quit(this));
		_toolaBar.add(_quitButton);

		// TODO Inicializar _fc con una instancia de JFileChooser. Para que siempre
		// abre en la carpeta de ejemplos puedes usar:
		//
		this._fc = new JFileChooser();
		_fc.setCurrentDirectory(new File(System.getProperty("user.dir") + "/resources/examples"));
		// TODO Inicializar _changeRegionsDialog con instancias del di�logo de cambio
		// de regiones
		_changeRegionsDialog = new ChangeRegionsDialog(_ctrl);

	}
	// TODO el resto de m�todos van aqu�

	public void actionPerformed(ActionEvent e) {
		if (e.getSource() == _openButton) {
			int a = _fc.showOpenDialog(ViewUtils.getWindow(this));
			if (a == JFileChooser.APPROVE_OPTION) {
				File f = _fc.getSelectedFile();
				try {
					FileInputStream in = new FileInputStream(f);
					JSONObject jsonInput = new JSONObject(new JSONTokener(in));
					if (jsonInput.has("cols") && jsonInput.has("rows") && jsonInput.has("width")
							&& jsonInput.has("height")) {// &&
						int cols = jsonInput.getInt("cols");
						int rows = jsonInput.getInt("rows");
						int width = jsonInput.getInt("width");
						int height = jsonInput.getInt("height");
						_ctrl.reset(cols, rows, width, height);
						_ctrl.load_data(jsonInput);
					} else {
						throw new IllegalArgumentException("The file must have a width");
					}
				} catch (FileNotFoundException e1) {
					e1.printStackTrace();
				}
			}
		}

		if (e.getSource() == _viewerButton) {
			new MapWindow((JFrame) ViewUtils.getWindow(this), _ctrl);
		}

		if (e.getSource() == _regionsButton) {
			_changeRegionsDialog.open(ViewUtils.getWindow(this));
		}

		if (e.getSource() == _runButton) {
			_openButton.setEnabled(false);
			_regionsButton.setEnabled(false);
			_viewerButton.setEnabled(false);
			_runButton.setEnabled(false);
			_quitButton.setEnabled(false);
			_stopped = false;

			double deltaTime = (Double.parseDouble(dt.getText()));
			run_sim(Integer.parseInt(steps.getValue().toString()), deltaTime);
		}
		if (e.getSource() == _stopButton) {
			_stopped = true;
		}
	}

	private void run_sim(int n, double dt) {
		if (n > 0 && !_stopped) {
			try {
				_ctrl.advance(dt);
				SwingUtilities.invokeLater(() -> run_sim(n - 1, dt));
			} catch (Exception e) {
				// TODO llamar a ViewUtils.showErrorMsg con el mensaje de error que corresponda
				ViewUtils.showErrorMsg(e.getMessage());
				// activar todos los botones
				_openButton.setEnabled(true);
				_regionsButton.setEnabled(true);
				_viewerButton.setEnabled(true);
				_runButton.setEnabled(true);
				_quitButton.setEnabled(true);
				_stopped = true;
				return;
			}

		} else {
			// activar todos los botones
			_openButton.setEnabled(true);
			_regionsButton.setEnabled(true);
			_viewerButton.setEnabled(true);
			_runButton.setEnabled(true);
			_quitButton.setEnabled(true);
			_stopped = true;
		}
	}

	@Override
	public void onRegister(double time, MapInfo map, List<AnimalInfo> animals) {
		// TODO Auto-generated method stub
	}

	@Override
	public void onReset(double time, MapInfo map, List<AnimalInfo> animals) {
		// TODO Auto-generated method stub

	}

	@Override
	public void onAnimalAdded(double time, MapInfo map, List<AnimalInfo> animals, AnimalInfo a) {
		// TODO Auto-generated method stub

	}

	@Override
	public void onRegionSet(int row, int col, MapInfo map, RegionInfo r) {
		// TODO Auto-generated method stub

	}

	@Override
	public void onAvanced(double time, MapInfo map, List<AnimalInfo> animals, double dt) {
		// TODO Auto-generated method stub

	}

}
