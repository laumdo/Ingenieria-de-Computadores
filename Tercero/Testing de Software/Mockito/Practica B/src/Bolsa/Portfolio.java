package Bolsa;

import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;


public class Portfolio {
	
	
  // Para cada stock (representado por un simbolo) se dispone de una lista de las compras realizadas.
	
   private Map<String, List<Stock>> stockMap = new HashMap<String, List<Stock>>();
   
   // Compra de stocks: Si existe el stock en la cartera (portfolio) se añade una nueva entrada a la lista asociada a su símbolo
   //e.o.c. se crea una nueva entrada en la cartera para dicho simbolo y se incluye el nuevo stock
   
   public void comprar(Stock stock) {
	   List<Stock> stocks = stockMap.get(stock.getSimbolo());
	   if(stocks != null) {
		   stocks.add(stock);
	   }else {
		   stocks = new ArrayList<Stock>();
		   stocks.add(stock);
		   stockMap.put(stock.getSimbolo(), stocks);
	   }
   }
   
   // Venta de stocks: Si no existe el stock en la cartera se lanza una excepcion
   //e.o.c. se eliminan tantas entradas como indique cantidad. 
   
   public void vender(Stock stock, int cantidad) {
	   List<Stock> stocks = stockMap.get(stock.getSimbolo());
	   if(stocks == null) {
		   throw new IllegalStateException(stock.getSimbolo()+" no incluido en la cartera");
	   }
	   
	   if(stocks.size() < cantidad) {
		   cantidad = stocks.size();
	   }
	   
	   for(int i=cantidad-1; i >=0; i--) {
		   stocks.remove(i);
	   }
   }
   
   // Devuelve cero si no hay en la cartera el stock solicitado.
   // e.o.c. devuelve el precio medio de los stocks comprados
   
   public BigDecimal getAvgPrecio(Stock stock) {
	   List<Stock> stocks = stockMap.get(stock.getSimbolo());
	   BigDecimal avgPrecio = BigDecimal.ZERO;
	   if(stocks != null) {
		   for(Stock aStock:stocks) {
			   avgPrecio = avgPrecio.add(aStock.getPrecio());
		   }
		   
		   avgPrecio = avgPrecio.divide(new BigDecimal(stocks.size()));
	   }
	   
	   return avgPrecio;
   }
   
   // Devuelve el valor total de la cartera
   
   public BigDecimal getValorActual() {
	   BigDecimal actPrecio = BigDecimal.ZERO;
	   for(String stockId: stockMap.keySet()) {
		   for(Stock stock:stockMap.get(stockId)) {
			   actPrecio = actPrecio.add(stock.getPrecio());
		   }
	   }
	   
	   return actPrecio;
   }
   
}
