package simulator.model;

import java.util.List;

import simulator.misc.Utils;
import simulator.misc.Vector2D;

public class Sheep extends Animal {
	private Animal _danger_source;
	private SelectionStrategy _danger_strategy;

	public Animal get_danger_source() {
		return _danger_source;
	}

	public SelectionStrategy get_danger_strategy() {
		return _danger_strategy;
	}

	private final static double CAMPO_VISTA_INICIAL = 40.0;
	private final static double VELOCIDAD = 35.0;
	private final static double DESEO = 65.0;
	private final static double CONSTANTE5 = 2.0;
	private final static double CONSTANTE6 = 1.2;
	private final static double EDAD = 8.0;
	private final static double PROBABILIDAD = 0.9;

	public Sheep(SelectionStrategy mate_strategy, SelectionStrategy danger_strategy, Vector2D pos) {// constructora 1
		super("Sheep", Diet.HERBIVORE, CAMPO_VISTA_INICIAL, VELOCIDAD, mate_strategy, pos);
		this._danger_strategy = danger_strategy;
	}

	protected Sheep(Sheep p1, Animal p2) {// constructora 2
		super(p1, p2);
		this._danger_strategy = p1._danger_strategy;
		this._danger_source = null;

	}

	@Override
	public void update(double dt) {
		if (this._state == State.DEAD) {
			return;
		}
		// Actualizar el objeto según el estado del animal
		if (this._state == State.NORMAL) {
			this.update_Normal(dt);
		} else if (this._state == State.DANGER) {
			this.update_Danger(dt);
		} else if (this._state == State.MATE) {
			this.update_Mate(dt);
		}

		// Si pos está fuera, ajustarla y cambiar estado a NORMAL
		if (this.estaFuera()) {
			this.ajustarPos();
			this.cambio_estado_Normal();
		}
		// si se queda sin energia o pasa de edad muere
		if (this._energy == 0.0 || this._age > EDAD) {
			this._state = State.DEAD;
		}
		// si no esta muerto
		if (this._state != State.DEAD) {
			this._energy += this._region_mngr.get_food(this, dt); // Pedir comida
			this._energy = Utils.constrain_value_in_range(this._energy, RANGO_MIN, RANGO_MAX);
		}
	}

	private void update_Normal(double dt) {
		// Avanzar el animal
		this.avanzar(dt);

		if (this._danger_source == null) { // buscar un nuevo animal q se considere peligroso
			List<Animal> l_danger = this._region_mngr.get_animals_in_range(this, (Animal a) -> {
				return a.get_diet() == Diet.CARNIVORE;
			});

			this._danger_source = this._danger_strategy.select(this, l_danger);
		}
		// Cambio de estado:
		if (this._danger_source != null) {
			this.cambio_estado_Danger();
		} else if (this._danger_source == null && this._desire > DESEO) {
			this.cambio_estado_Mate();
		}
	}

	private void update_Danger(double dt) {
		if (this._danger_source != null && this._danger_source.get_state() == State.DEAD) {
			this._danger_source = null;
		}

		if (this._danger_source == null) { // avanzar como punto 1 de NORMAL
			this.avanzar(dt);
		} else {
			this._dest = _pos.plus(_pos.minus(_danger_source.get_position()).direction());
			this.move(CONSTANTE5 * _speed * dt * Math.exp((_energy - CONSTANTE1) * CONSTANTE2));
			this._age += dt;
			this._energy -= CONSTANTE3 * CONSTANTE6 * dt;
			this._energy = Utils.constrain_value_in_range(this._energy, RANGO_MIN, RANGO_MAX);
			this._desire += CONSTANTE4 * dt;
			this._desire = Utils.constrain_value_in_range(this._desire, RANGO_MIN, RANGO_MAX);
		}

		if (this._danger_source == null
				|| this._danger_source.get_position().distanceTo(this._pos) > this._sight_range) { // danger source no
																									// está en el campo
																									// visual
			// buscar un nuevo animal q se considere peligroso
			List<Animal> l_danger = this._region_mngr.get_animals_in_range(this, (Animal a) -> {
				return a.get_diet() == Diet.CARNIVORE;

			});
			this._danger_source = this._danger_strategy.select(this, l_danger);
			// Cambio de estado
			if (this._danger_source == null) {
				if (this._desire < DESEO)
					this.cambio_estado_Normal();
				else
					this.cambio_estado_Mate();
			}
		}
	}

	private void update_Mate(double dt) {
		if (this._mate_target != null && (this._mate_target.get_state() == State.DEAD || this._pos
				.distanceTo(this._mate_target.get_position()) > this._sight_range /* fuera del campo visual */)) {
			this._mate_target = null;
		}

		if (this._mate_target == null) {
			// buscar un animal para emparejarse
			List<Animal> l_mate = this._region_mngr.get_animals_in_range(this, (Animal a) -> {
				return a.get_genetic_code() == this._genetic_code;
			});
			if (l_mate.isEmpty()) {
				// si no encuentra:
				this.avanzar(dt);
			} else {
				this._mate_target = this._mate_strategy.select(this, l_mate);
			}

		} else {
			this._dest = this._mate_target.get_position();
			this.move(CONSTANTE5 * _speed * dt * Math.exp((_energy - CONSTANTE1) * CONSTANTE2));
			this._age += dt;
			this._energy -= CONSTANTE3 * CONSTANTE6 * dt;
			this._energy = Utils.constrain_value_in_range(this._energy, RANGO_MIN, RANGO_MAX);
			this._desire += CONSTANTE4 * dt;
			this._desire = Utils.constrain_value_in_range(this._desire, RANGO_MIN, RANGO_MAX);
			if (this._pos.distanceTo(this._mate_target.get_position()) < DISTANCIA) {
				this._desire = 0.0;
				this._mate_target.set_desire(0.0);
				if (this._baby == null && Utils._rand.nextDouble() < PROBABILIDAD) {
					this._baby = new Sheep(this, _mate_target);
				}
				this._mate_target = null;
			}
		}

		if (this._danger_source == null) {// busca un animal peligroso
			List<Animal> l_danger = this._region_mngr.get_animals_in_range(this, (Animal a) -> {
				return a.get_diet() == Diet.CARNIVORE;
			});
			this._danger_source = this._danger_strategy.select(this, l_danger);
		}
		// cambio de estado
		if (this._danger_source != null) {
			this.cambio_estado_Danger();
		} else if (this._danger_source == null && this._desire < DESEO) {
			this.cambio_estado_Normal();
		}
	}

	private void cambio_estado_Normal() {
		this._state = State.NORMAL;
		this._danger_source = null;
		this._mate_target = null;
	}

	private void cambio_estado_Mate() {
		this._state = State.MATE;
		this._danger_source = null;
	}

	private void cambio_estado_Danger() {
		this._state = State.DANGER;
		this._mate_target = null;
	}
}
