package simulator.view;

import java.awt.BorderLayout;
import java.awt.Color;

import javax.swing.BorderFactory;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.border.Border;
import javax.swing.border.TitledBorder;
import javax.swing.table.TableModel;

public class InfoTable extends JPanel {
	String _title;
	TableModel _tableModel;

	InfoTable(String title, TableModel tableModel) {
		_title = title;
		_tableModel = tableModel;
		initGUI();
	}

	private void initGUI() {
		// Cambiar el layout del panel a BorderLayout()
		this.setLayout(new BorderLayout());
		// Añadir un borde con título al JPanel, con el texto _title
		Border borde = BorderFactory.createLineBorder(Color.black, 1);
		this.setBorder(BorderFactory.createTitledBorder(borde, _title, TitledBorder.CENTER, TitledBorder.TOP));
		// Añadir un JTable (con barra de desplazamiento vertical) que use _tableModel
		JTable jt = new JTable(_tableModel);
		JScrollPane scroll = new JScrollPane(jt, JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED,
				JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
		this.add(scroll);
	}

}
