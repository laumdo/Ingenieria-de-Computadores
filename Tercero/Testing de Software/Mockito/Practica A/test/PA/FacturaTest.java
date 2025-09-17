package PA;
import static org.mockito.Mockito .*;

import org.junit.Test;

public class FacturaTest {

	private PrinterService ps=mock(PrinterService.class);
	private EmailService email=mock(EmailService.class);
	private EmisorFactura emi=new EmisorFactura(ps,email);
	private Factura fact=mock(Factura.class);
	
	@Test
	public void test1() {
		//Cliente cl=mock(Cliente.class); //hacer new?
		Cliente cl=new Cliente("hePerdidoLaCartera@ucm.es", true);
		
		//when(cl.prefiereEmails()).thenReturn(true);
		//when(cl.getEmail()).thenReturn("hePerdidoLaCartera@ucm.es");
		emi.emitirFactura(fact, cl);
		
		verify(email).sendFactura(fact, "hePerdidoLaCartera@ucm.es");
	}
	
	@Test
	public void test2() {
		//Cliente cl=mock(Cliente.class);
		Cliente cl=new Cliente("hePerdidoLaCartera@ucm.es", false);
		
		//when(cl.prefiereEmails()).thenReturn(false);
		emi.emitirFactura(fact, cl);
		
		verify(ps).printFactura(fact);
	}
}
