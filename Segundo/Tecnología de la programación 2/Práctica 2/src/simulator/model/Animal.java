package simulator.model;

import org.json.JSONObject;

import simulator.misc.Utils;
import simulator.misc.Vector2D;

public abstract class Animal implements Entity, AnimalInfo {

	private final static double TOLERANCE = 0.1;
	private final static double ENERGY = 100.0;
	private final static double CONSTANTE5 = 60.0;
	private final static double CONSTANTE6 = 0.2;
	protected final static double DISTANCIA = 8.0;
	protected final static double CONSTANTE1 = 100.0;
	protected final static double CONSTANTE2 = 0.007;
	protected final static double CONSTANTE3 = 20.0;
	protected final static double CONSTANTE4 = 40.0;
	protected final static double RANGO_MIN = 0.0;
	protected final static double RANGO_MAX = 100.0;
	protected String _genetic_code;
	protected Diet _diet;
	protected State _state;
	protected Vector2D _pos;
	protected Vector2D _dest;
	protected double _energy;
	protected double _speed; // velocidad
	protected double _age;
	protected double _desire;
	protected double _sight_range; // campo de vista
	protected Animal _mate_target;
	protected Animal _baby;
	protected AnimalMapView _region_mngr;
	protected SelectionStrategy _mate_strategy;

	protected Animal(String genetic_code, Diet diet, double sight_range, double init_speed,
			SelectionStrategy mate_strategy, Vector2D pos) {// constructora 1
		this._genetic_code = genetic_code;
		this._diet = diet;
		this._sight_range = sight_range;
		this._pos = pos;
		this._mate_strategy = mate_strategy;
		this._speed = Utils.get_randomized_parameter(init_speed, TOLERANCE);
		this._state = State.NORMAL;
		this._energy = ENERGY;
		this._desire = 0.0;
		this._dest = null;
		this._mate_target = null;
		this._baby = null;
		this._region_mngr = null;
		// excepciones
		if (genetic_code == "") {// si el codigo genetico esta vacio salta excepcion
			throw new IllegalArgumentException("Mustn't be an empty String");
		}
		if (sight_range < 0 || init_speed < 0) {// si sight_range o init_speed son menores que 0 salta excepcion
			throw new IllegalArgumentException("Must be a positive parameter");
		}
		if (mate_strategy == null) {// si mate strategy es nulo salta excepcion
			throw new IllegalArgumentException("Mustn't be null");
		}
	}

	// getters
	public String get_genetic_code() {
		return _genetic_code;
	}

	public Diet get_diet() {
		return _diet;
	}

	public Vector2D get_position() {
		return _pos;
	}

	protected Vector2D get_dest() {
		return _dest;
	}

	public double get_energy() {
		return _energy;
	}

	public double get_speed() {
		return _speed;
	}

	public double get_age() {
		return _age;
	}

	protected double get_desire() {
		return _desire;
	}

	public double get_sight_range() {
		return _sight_range;
	}

	protected Animal get_mate_target() {
		return _mate_target;
	}

	protected Animal get_baby() {
		return _baby;
	}

	@Override
	public State get_state() {
		return this._state;
	}

	protected AnimalMapView get_region_mngr() {
		return _region_mngr;
	}

	protected SelectionStrategy get_mate_strategy() {
		return _mate_strategy;
	}

	public Vector2D get_destination() {
		return this._dest;
	}

	// setters

	protected void set_state(State st) {
		this._state = st;
	}

	protected void set_desire(double desire) {
		this._desire = desire;
	}

	protected Animal(Animal p1, Animal p2) {// constructora 2
		this._dest = null;
		this._mate_target = null;
		this._mate_strategy = p2.get_mate_strategy();
		this._baby = null;
		this._region_mngr = null;
		this._state = State.NORMAL;
		this._desire = 0.0;
		this._genetic_code = p1.get_genetic_code();
		this._diet = p1.get_diet();
		this._energy = (p1.get_energy() + p2.get_energy()) / 2;
		this._pos = p1.get_position()
				.plus(Vector2D.get_random_vector(-1, 1).scale(CONSTANTE5 * (Utils._rand.nextGaussian() + 1)));
		this._sight_range = Utils.get_randomized_parameter((p1.get_sight_range() + p2.get_sight_range()) / 2,
				CONSTANTE6);
		this._speed = Utils.get_randomized_parameter((p1.get_speed() + p2.get_speed()) / 2, CONSTANTE6);
	}

	// metodos
	public boolean is_pregnant() {// devuelvo true si baby no es nulo
		return this._baby != null;
	}

	void init(AnimalMapView reg_mngr) {
		this._region_mngr = reg_mngr;
		if (this._pos == null) {// si no tengo posicion le doy una random
			this._pos = new Vector2D(Utils._rand.nextDouble(_region_mngr.get_width() - 1),
					Utils._rand.nextDouble(_region_mngr.get_height() - 1));
		} else {// si la posicion esra fuera del mapa la ajusto
			if (this.estaFuera())
				this.ajustarPos();
		}
		// le doy un nuevo destino
		this._dest = new Vector2D(Utils._rand.nextDouble(_region_mngr.get_width() - 1),
				Utils._rand.nextDouble(_region_mngr.get_height() - 1));
	}

	Animal deliver_baby() {// devuelvo el bebe que tengo
		Animal baby_aux = this._baby;
		this._baby = null;
		return baby_aux;
	}

	protected void move(double speed) {// metodo para el movimiento
		this._pos = _pos.plus(_dest.minus(_pos).direction().scale(speed));
	}

	public JSONObject as_JSON() {
		JSONObject j = new JSONObject();
		j.put("pos", _pos);
		j.put("gcode", _genetic_code);
		j.put("diet", _diet);
		j.put("state", _state);
		return j;
	}

	protected void ajustarPos() {// ajusto la posicion para que si sale por un lado aparezca por el contrario
		double x = this._pos.getX(), y = this._pos.getY();

		while (x >= _region_mngr.get_width())
			x = (x - _region_mngr.get_width());
		while (x < 0)
			x = (x + _region_mngr.get_width());
		while (y >= _region_mngr.get_height())
			y = (y - _region_mngr.get_height());
		while (y < 0)
			y = (y + _region_mngr.get_height());
		this._pos = new Vector2D(x, y);
	}

	protected boolean estaFuera() {// metodo para saber si esta fuera del mapa
		return this._pos.getX() < 0 || this._pos.getX() >= this._region_mngr.get_width() || this._pos.getY() < 0
				|| this._pos.getY() >= this._region_mngr.get_height();
	}

	protected void avanzar(double dt) {// metodo para cuando tenga que avanzar
		if (this._pos.distanceTo(_dest) < DISTANCIA) {// si la distancia a un destino es menor que mi valor actualizo mi
														// destino con uno aleatorio
			this._dest = new Vector2D(Utils._rand.nextDouble(_region_mngr.get_width() - 1),
					Utils._rand.nextDouble(_region_mngr.get_height() - 1));
		}
		this.move(_speed * dt * Math.exp((this._energy - CONSTANTE1) * CONSTANTE2));
		this._age += dt;
		this._energy -= CONSTANTE3 * dt;
		this._energy = Utils.constrain_value_in_range(this._energy, RANGO_MIN, RANGO_MAX);
		this._desire += CONSTANTE4 * dt;
		this._desire = Utils.constrain_value_in_range(this._desire, RANGO_MIN, RANGO_MAX);
	}
}
