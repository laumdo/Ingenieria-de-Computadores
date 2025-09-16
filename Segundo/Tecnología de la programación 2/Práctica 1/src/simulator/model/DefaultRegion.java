package simulator.model;

public class DefaultRegion extends Region {

	private final static double CONSTANTE1 = 60.0;
	private final static double CONSTANTE2 = 5.0;
	private final static double CONSTANTE3 = 2.0;

	@Override
	public double get_food(Animal a, double dt) {
		if (a.get_diet() == Diet.CARNIVORE)
			return 0.0;
		else {
			int n = (int) getAnimals().stream().filter((Animal e) -> {
				return e.get_diet() == Diet.HERBIVORE;
			}).count();
			return CONSTANTE1 * Math.exp(-Math.max(0, n - CONSTANTE2) * CONSTANTE3) * dt;
		}
	}

	@Override
	public void update(double dt) {

	}

}
