package simulator.view;

import java.awt.Dimension;
import java.awt.Frame;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.List;

import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.DefaultComboBoxModel;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.table.DefaultTableModel;

import org.json.JSONArray;
import org.json.JSONObject;

import simulator.control.Controller;
import simulator.launcher.Main;
import simulator.model.AnimalInfo;
import simulator.model.EcoSysObserver;
import simulator.model.MapInfo;
import simulator.model.RegionInfo;

public class ChangeRegionsDialog extends JDialog implements EcoSysObserver {

	private DefaultComboBoxModel<String> _regionsModel;
	private DefaultComboBoxModel<String> _fromRowModel;
	private DefaultComboBoxModel<String> _toRowModel;
	private DefaultComboBoxModel<String> _fromColModel;
	private DefaultComboBoxModel<String> _toColModel;

	private DefaultTableModel _dataTableModel;
	private Controller _ctrl;
	private List<JSONObject> _regionsInfo;

	private String[] _headers = { "Key", "Value", "Description" };
	// TODO en caso de ser necesario, a�adir los atributos aqu�
	private JComboBox<String> cbRegion;
	private int _selectedRegionsIndex;
	private JSONObject data;
	private JSONObject info;

	ChangeRegionsDialog(Controller ctrl) {
		super((Frame) null, true);
		_ctrl = ctrl;
		initGUI();
		ctrl.addObserver(this);
	}

	private void initGUI() {
		setTitle("Change Regions");
		JPanel mainPanel = new JPanel();
		mainPanel.setLayout(new BoxLayout(mainPanel, BoxLayout.Y_AXIS));
		setContentPane(mainPanel);

		// Ccrea varios paneles para organizar los componentes visuales en el
		// dialogo, y a�adelos al mainpanel. P.ej., uno para el texto de ayuda,
		// uno para la tabla, uno para los combobox, y uno para los botones.

		// Crear el texto de ayuda que aparece en la parte superior del di�logo y
		// a�adirlo al panel correspondiente di�logo (Ver el apartado Figuras)
		JLabel label = new JLabel();
		label.setText("<html><p>Select a region type, the rows/cols interval, and provide values for the parametes"
				+ " in the <b>Value column</b> (default values are used for parameters with"
				+ " no value).</p></html>");
		label.setAlignmentX(CENTER_ALIGNMENT);
		mainPanel.add(label);
		mainPanel.add(Box.createRigidArea(new Dimension(0, 20)));

		// _regionsInfo se usar� para establecer la informaci�n en la tabla
		_regionsInfo = Main.regionsFactory.get_info();

		// _dataTableModel es un modelo de tabla que incluye todos los par�metros de la
		// region
		_dataTableModel = new DefaultTableModel() {
			@Override
			public boolean isCellEditable(int row, int column) {
				// TODO hacer editable solo la columna 1
				return column == 1;
			}
		};
		_dataTableModel.setColumnIdentifiers(_headers);

		// Crear un JTable que use _dataTableModel, y a�adirlo al di�logo
		JTable table = new JTable(_dataTableModel);
		JScrollPane scrollPane = new JScrollPane(table, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
				JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
		table.setPreferredSize(new Dimension(400, 400));
		mainPanel.add(scrollPane);

		// Crea un panel para a�adir los combobox
		JPanel comboPanel = new JPanel();
		mainPanel.add(comboPanel);

		comboPanel.add(new JLabel("Region Type: "));

		// _regionsModel es un modelo de combobox que incluye los tipos de regiones
		_regionsModel = new DefaultComboBoxModel<>();

		// TODO a�adir la descripci�n de todas las regiones a _regionsModel, para eso
		// usa la clave �desc� o �type� de los JSONObject en _regionsInfo,
		// ya que estos nos dan informaci�n sobre lo que puede crear la factor�a.
		for (JSONObject obj : _regionsInfo) {
			_regionsModel.addElement(obj.getString("desc")); // FALTA CLAVE TYPE
		}

		// Crear un combobox que use _regionsModel y a�adirlo al di�logo.
		JComboBox<String> cbRegionType = new JComboBox<>(_regionsModel);
		cbRegionType.setPreferredSize(new Dimension(200, 30));
		cbRegionType.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				_selectedRegionsIndex = cbRegionType.getSelectedIndex();
				JSONObject info = _regionsInfo.get(_selectedRegionsIndex);
				if (info.has("data")) {
					JSONObject data = info.getJSONObject("data");

					Object[] data2 = new Object[3];
					for (int i = 0; i < _dataTableModel.getRowCount(); i++) {
						_dataTableModel.removeRow(i);
					}

					_dataTableModel.setRowCount(0);
					for (String key : data.keySet()) {
						data2[0] = key;
						data2[1] = "";
						data2[2] = data.get(key);
						_dataTableModel.addRow(data2);
					}
				}
			}

		});
		comboPanel.add(cbRegionType);

		info = _regionsInfo.get(0);
		if (info.has("data")) {
			data = info.getJSONObject("data");
			for (String s : data.keySet()) {
				Object[] newRowData = { s, "", data.get(s) };
				_dataTableModel.addRow(newRowData);
			}
		}
		cbRegionType.addActionListener((e) -> {
			int rowCount = _dataTableModel.getRowCount();

			// Remove rows from previous law
			for (int i = rowCount - 1; i >= 0; i--) {
				_dataTableModel.removeRow(i);
			}

			// Add rows for selected law
			_selectedRegionsIndex = cbRegionType.getSelectedIndex();
			JSONObject lawInfo = _regionsInfo.get(_selectedRegionsIndex);
			if (lawInfo.has("data")) {
				JSONObject lawData = lawInfo.getJSONObject("data");
				for (String key : lawData.keySet()) {
					Object[] rowData = { key, "", lawData.get(key) };
					_dataTableModel.addRow(rowData);
				}
			}
		});

		// Crear 4 modelos de combobox para _fromRowModel, _toRowModel, _fromColModel y
		// _toColModel.
		comboPanel.add(new JLabel("Row from/to:"));
		_fromRowModel = new DefaultComboBoxModel<>();
		cbRegion = new JComboBox<String>(_fromRowModel);
		comboPanel.add(cbRegion);

		_toRowModel = new DefaultComboBoxModel<>();
		cbRegion = new JComboBox<String>(_toRowModel);
		comboPanel.add(cbRegion);

		comboPanel.add(new JLabel("Column from/to:"));
		_fromColModel = new DefaultComboBoxModel<>();
		cbRegion = new JComboBox<String>(_fromColModel);
		comboPanel.add(cbRegion);

		_toColModel = new DefaultComboBoxModel<>();
		cbRegion = new JComboBox<String>(_toColModel);
		comboPanel.add(cbRegion);

		// TODO crear los botones OK y Cancel y a�adirlos al di�logo.
		JButton okButton = new JButton("OK");
		JButton cancelButton = new JButton("Cancel");
		// Add action listeners to buttons
		okButton.addActionListener((e) -> {
			try {
				JSONObject tableObject = new JSONObject(dataTable());
				JSONObject specObject = new JSONObject();
				specObject.put("data", tableObject);
				specObject.put("type", _regionsInfo.get(_selectedRegionsIndex).getString("type"));

				System.out.println(specObject);
				
				JSONObject finalObject = new JSONObject();
				finalObject.put("spec", specObject);
				
				JSONArray rows = new JSONArray();
				rows.put(Integer.parseInt(_fromRowModel.getSelectedItem().toString()));
				rows.put(Integer.parseInt(_toRowModel.getSelectedItem().toString()));
				finalObject.put("row", rows);
				
				JSONArray cols = new JSONArray();
				cols.put(Integer.parseInt(_fromColModel.getSelectedItem().toString()));
				cols.put(Integer.parseInt(_toColModel.getSelectedItem().toString()));
				
				finalObject.put("col", cols);
				_ctrl.set_regions(finalObject);
				// _status = 1;
				setVisible(false);
			} catch (Exception ex) {
				ViewUtils.showErrorMsg("Error saving entered data");
			}
		});

		cancelButton.addActionListener((e) -> {
			setVisible(false);
		});

		// Add buttons to panel
		JPanel buttonPanel = new JPanel();
		buttonPanel.add(okButton);
		buttonPanel.add(cancelButton);
		mainPanel.add(buttonPanel);

		setPreferredSize(new Dimension(800, 400)); // puedes usar otro tama�o
		pack();
		setResizable(false);
		setVisible(false);
	}

	public void open(Frame parent) {
		setLocation(//
				parent.getLocation().x + parent.getWidth() / 2 - getWidth() / 2, //
				parent.getLocation().y + parent.getHeight() / 2 - getHeight() / 2);
		pack();
		setVisible(true);
	}

	@Override
	public void onRegister(double time, MapInfo map, List<AnimalInfo> animals) {
		// TODO Auto-generated method stub
		updateMap(map);
	}

	@Override
	public void onReset(double time, MapInfo map, List<AnimalInfo> animals) {
		// TODO Auto-generated method stub
		updateMap(map);
	}

	@Override
	public void onAnimalAdded(double time, MapInfo map, List<AnimalInfo> animals, AnimalInfo a) {

	}

	@Override
	public void onRegionSet(int row, int col, MapInfo map, RegionInfo r) {
		// TODO Auto-generated method stub

	}

	@Override
	public void onAvanced(double time, MapInfo map, List<AnimalInfo> animals, double dt) {
		// TODO Auto-generated method stub

	}

	public String dataTable() {
		StringBuilder stringB = new StringBuilder();

		stringB.append('{');
		for (int i = 0; i < this._dataTableModel.getRowCount(); i++) {
			String s = _dataTableModel.getValueAt(i, 0).toString();
			String st = _dataTableModel.getValueAt(i, 1).toString();

			if (st.isEmpty() == false) {
				stringB.append('"');
				stringB.append(s);
				stringB.append('"');
				stringB.append(':');
				stringB.append(st);
				stringB.append(',');
			}
		}

		if (stringB.length() > 1) {
			stringB.deleteCharAt(stringB.length() - 1);
		}

		stringB.append('}');

		System.out.println(stringB);
		return stringB.toString();
	}

	private void updateMap(MapInfo map) {
		_fromColModel.removeAllElements();
		_fromRowModel.removeAllElements();
		_toRowModel.removeAllElements();
		_toColModel.removeAllElements();
		for (Integer i = 0; i < map.get_cols(); i++) {
			_fromColModel.addElement(i.toString());
			_toColModel.addElement(i.toString());
		}
		for (Integer i = 0; i < map.get_rows(); i++) {
			_fromRowModel.addElement(i.toString());
			_toRowModel.addElement(i.toString());
		}
	}
}
