package Bolsa;


import java.math.*;


public class StockBroker {
	
	private final static BigDecimal LIMIT = new BigDecimal("0.10");
	private final AnalistaMercado analista;
	
	public StockBroker(AnalistaMercado analista) {
		this.analista = analista;
	}
	
	// Recupera el precio actual del stock en el mercado (livestock) y calcula la diferencia con el precio medio de la cartera (portfolio)
	// si es mayor que el límite se venden 10 y si es menor se compra 1
	
	void perform(Portfolio portfolio,Stock stock) {
		
		Stock liveStock = analista.getCotizacion(stock.getSimbolo());
        BigDecimal avgPrecio = portfolio.getAvgPrecio(stock);
        
		BigDecimal precioGanancia = liveStock.getPrecio().subtract(avgPrecio);
		BigDecimal porcentGanancia = precioGanancia.divide(avgPrecio, 2, RoundingMode.FLOOR);
		
		int val = porcentGanancia.compareTo(LIMIT);
		
		if(val>0) {
			portfolio.vender(stock, 10);
		//
			portfolio.comprar(stock);
		}
	}
	
	
}
