package Bolsa;
import static org.mockito.Mockito .*;

import java.math.BigDecimal;

import org.junit.Test;

public class BolsaTest {

	AnalistaMercado am = mock(AnalistaMercado.class);
	Portfolio p = mock(Portfolio.class);
	Stock s = mock(Stock.class);
	StockBroker sb = new StockBroker(am);
	
	@Test
	public void compraTest() { 
		when(am.getCotizacion(s.getSimbolo())).thenReturn(new Stock("245","Adios",new BigDecimal("105.0")));
		when(p.getAvgPrecio(s)).thenReturn(new BigDecimal("100.0"));
		
		
		sb.perform(p, s);
		
		verify(p).comprar(s);
		
	}
	
	@Test
	public void ventaTest() { 
		when(am.getCotizacion(s.getSimbolo())).thenReturn(new Stock("245","Adios",new BigDecimal("115.0")));
		when(p.getAvgPrecio(s)).thenReturn(new BigDecimal("100.0"));
		
		
		sb.perform(p, s);
				
		verify(p).vender(s, 10);
		
	}
	
	@Test
	public void ningunaOperacionTest() {
		when(am.getCotizacion(s.getSimbolo())).thenReturn(new Stock("245","Adios",new BigDecimal("110.0")));
		when(p.getAvgPrecio(s)).thenReturn(new BigDecimal("100.0"));
		
		
		sb.perform(p, s);
		
		verify(p,never()).comprar(s);
		verify(p, never()).vender(s, 10);
		
	}
	
	@Test(expected = IllegalStateException.class)
	public void mercadoCaidoTest() throws Exception{
		when(am.getCotizacion(s.getSimbolo())).thenThrow(new IllegalStateException());
		sb.perform(p, s);
	}
	
}
