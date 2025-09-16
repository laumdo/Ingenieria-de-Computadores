package simulator.model;

import java.util.List;

import simulator.misc.Utils;
import simulator.misc.Vector2D;

public class Wolf extends Animal {
	private Animal _hunt_target; // animal al que quiere cazar
	private SelectionStrategy _hunting_strategy; // elegir un animal a cazar
	private final static double CAMPO_VISTA_INICIAL = 50.0;
	private final static double VELOCIDAD = 60.0;
	private final static double DISTANCIA = 8.0;
	private final static double CONSTANTE1 = 100.0;
	private final static double CONSTANTE2 = 0.007;
	private final static double RANGO_MIN = 0.0;
	private final static double RANGO_MAX = 100.0;
	private final static double CONSTANTE3 = 18.0;
	private final static double CONSTANTE4 = 30.0;
	private final static double ENERGIA = 50.0;
	private final static double DESEO = 65.0;
	private final static double CONSTANTE5 = 3.0;
	private final static double CONSTANTE6 = 1.2;
	private final static double PROBABILIDAD = 0.9;
	private final static double CONSTANTE7 = 10.0;
	private final static double EDAD = 14.0;

	public Wolf(SelectionStrategy mate_strategy, SelectionStrategy hunting_strategy, Vector2D pos) {// constructora 1
		super("Wolf", Diet.CARNIVORE, CAMPO_VISTA_INICIAL, VELOCIDAD, mate_strategy, pos);
		this._hunting_strategy = hunting_strategy;
	}

	protected Wolf(Wolf p1, Animal p2) {// constructora 2
		super(p1, p2);
		this._hunting_strategy = p1._hunting_strategy;
		this._hunt_target = null;
	}

	@Override
	public void update(double dt) {
		if (this._state == State.DEAD) {// si esta muerto se sale
			return;
		}
		// Actualizar según el estado del animal
		if (this._state == State.NORMAL) {
			this.update_Normal(dt);
		} else if (this._state == State.HUNGER) {
			this.update_Hunger(dt);
		} else if (this._state == State.MATE) {
			this.update_Mate(dt);
		}
		// Si pos está fuera, ajustarla y cambiar estado a NORMAL
		if (this.estaFuera()) {
			this.ajustarPos();
			cambio_estado_Normal();
		}
		if (this._energy == 0.0 || this._age > EDAD) {// si se queda sin energia o pasa de edad muere
			this._state = State.DEAD;

		}
		// Pedir comida
		if (this._state != State.DEAD) {
			this._energy += this._region_mngr.get_food(this, dt);
			this._energy = Utils.constrain_value_in_range(this._energy, RANGO_MIN, RANGO_MAX);
		}
	}

	private void update_Normal(double dt) {
		// Avanzar el animal
		this.avanzar(dt);
		// Cambio de estado
		if (this._energy < ENERGIA) {
			this.cambio_estado_Hunger();
		} else if (this._energy >= ENERGIA && this._desire > DESEO) {
			this.cambio_estado_Mate();
		}
	}

	private void update_Hunger(double dt) {
		if (this._hunt_target == null || (this._hunt_target != null && (this._hunt_target.get_state() == State.DEAD)
				|| (this._pos.distanceTo(this._hunt_target.get_position()) > this._sight_range))) {
			// buscar un animal para cazarlo
			List<Animal> l_hunt = this._region_mngr.get_animals_in_range(this, (Animal a) -> {
				return a.get_diet() == Diet.HERBIVORE;
			});

			this._hunt_target = this._hunting_strategy.select(this, l_hunt);
		}

		if (this._hunt_target == null) {
			this.avanzar(dt);
		} else {
			this._dest = _hunt_target.get_position();
			this.move(CONSTANTE5 * _speed * dt * Math.exp((_energy - CONSTANTE1) * CONSTANTE2));
			this._age += dt;
			this._energy -= CONSTANTE3 * CONSTANTE6 * dt;
			this._energy = Utils.constrain_value_in_range(this._energy, RANGO_MIN, RANGO_MAX);
			this._desire += CONSTANTE4 * dt;
			this._desire = Utils.constrain_value_in_range(this._desire, RANGO_MIN, RANGO_MAX);
			if (this._pos.distanceTo(this._hunt_target.get_position()) < DISTANCIA) {
				this._hunt_target.set_state(State.DEAD);
				this._hunt_target = null;
				this._energy += ENERGIA;
				this._energy = Utils.constrain_value_in_range(this._energy, RANGO_MIN, RANGO_MAX);
			}
		}

		// Cambio de estado
		if (this._energy > ENERGIA) {
			if (this._desire < DESEO) {
				this.cambio_estado_Normal();
			} else {
				this.cambio_estado_Mate();
			}
		}
	}

	private void update_Mate(double dt) {
		if (this._mate_target != null && (this._mate_target.get_state() == State.DEAD || (this._pos
				.distanceTo(this._mate_target.get_position()) > this._sight_range))) /* fuera del campo visual */ {
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
					this._baby = new Wolf(this, _mate_target);
				}
				this._energy -= CONSTANTE7;
				this._energy = Utils.constrain_value_in_range(this._energy, RANGO_MIN, RANGO_MAX);
				this._mate_target = null;
			}
		}
		// cambio de estado
		if (this._energy < ENERGIA) {
			this.cambio_estado_Hunger();
		} else if (this._energy >= ENERGIA && this._desire < DESEO) {
			this.cambio_estado_Normal();
		}
	}

	private void cambio_estado_Normal() {
		this._state = State.NORMAL;
		this._hunt_target = null;
		this._mate_target = null;
	}

	private void cambio_estado_Mate() {
		this._state = State.MATE;
		this._hunt_target = null;
	}

	private void cambio_estado_Hunger() {
		this._state = State.HUNGER;
		this._mate_target = null;
	}

}
