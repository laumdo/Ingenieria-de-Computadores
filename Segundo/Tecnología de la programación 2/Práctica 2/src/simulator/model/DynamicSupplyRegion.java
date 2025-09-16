package simulator.model;

import simulator.misc.Utils;

public class DynamicSupplyRegion extends Region {
	private double _food;
	private double _factor;

	private final static double CONSTANTE1 = 60.0;
	private final static double CONSTANTE2 = 5.0;
	private final static double CONSTANTE3 = 2.0;

	public DynamicSupplyRegion(double comida_ini, double factor_crecimiento) {// constructora
		this._food = comida_ini;
		this._factor = factor_crecimiento;
		// excepcion
		if (comida_ini < 0 || factor_crecimiento < 0) {// salta si algun valor es negativo
			throw new IllegalArgumentException("Must be a positive parameter");
		}
	}

	@Override
	public double get_food(Animal a, double dt) {
		if (a.get_diet() == Diet.CARNIVORE)
			return 0.0;
		else {
			int n = 0;
			for (Animal animal : getAnimals()) {
				if (animal.get_diet() == Diet.HERBIVORE)
					n++;
			}
			return Math.min(_food, CONSTANTE1 * Math.exp(-Math.max(0, n - CONSTANTE2) * CONSTANTE3) * dt);
		}
	}

	@Override
	public void update(double dt) {
		if (Utils._rand.nextDouble() < 0.5) {
			this._food += dt * _factor;
		}

	}

	public String toString() {
		return "Dynamic region";
	}
}
