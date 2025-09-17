package Bolsa;

import java.math.BigDecimal;

public class Stock {
	private final String simbolo;
	private final String nombre;
	private BigDecimal precio;

	public Stock(String id, String nombre, BigDecimal precio) {
		this.simbolo = id;
		this.nombre = nombre;
		this.precio = precio;
	}

	public String getSimbolo() {
		return simbolo;
	}

	public String getNombre() {
		return nombre;
	}

	public BigDecimal getPrecio() {
		return precio;
	}
	
	public void updatePrecio(BigDecimal newPrecio) {
		this.precio = newPrecio;
	}
}
