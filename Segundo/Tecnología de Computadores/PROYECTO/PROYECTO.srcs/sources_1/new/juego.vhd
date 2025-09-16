library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity juego is
   Port ( 
       rst: IN std_logic; --boton
       clk: IN std_logic;
       boton: IN std_logic; --boton (load)
       opcion: IN std_logic_vector(1 downto 0); --switches
       leds: OUT std_logic_vector(15 downto 0); --gana o pierde
       maquina: OUT std_logic_vector(1 downto 0); --num aleatorio por displays, QUITAR?
       salida_display: OUT std_logic_vector(6 downto 0); --display
       salida_display_enable: OUT std_logic_vector(3 downto 0) --display
   );
end juego;

architecture Behavioral of juego is
    type ESTADOS is (S0,S1,S2,S3);
    signal ESTADO, SIG_ESTADO: ESTADOS;
    component displays is
            Port ( 
                rst : in STD_LOGIC;
                clk : in STD_LOGIC;       
                digito_0 : in  STD_LOGIC_VECTOR (1 downto 0);
                digito_1 : in  STD_LOGIC_VECTOR (1 downto 0);
                digito_2 : in  STD_LOGIC_VECTOR (1 downto 0);
                digito_3 : in  STD_LOGIC_VECTOR (1 downto 0);
                aleatorio: in STD_LOGIC_VECTOR (1 downto 0);
                display : out  STD_LOGIC_VECTOR (6 downto 0);
                display_enable : out  STD_LOGIC_VECTOR (3 downto 0)
             );
    end component displays;
    component registro
        Port (
            rst, clk, load: IN std_logic;
            E: IN std_logic_vector(1 downto 0);
            S: OUT std_logic_vector(1 downto 0)
        );
    end component;
    component generadorAleatorio
         Port ( 
            clk: in std_logic;
            rst: in std_logic;
            enable: in std_logic;
            numero: out std_logic_vector(1 downto 0));
    end component;
    component comparador
        Port(E1, E2: IN std_logic_vector(1 downto 0);
            comp: IN std_logic;
            S, Empate: OUT std_logic
        );
    end component;
    component ganar is
        port (
            rst : in std_logic;
            clk : in std_logic;
            leds : out std_logic_vector(15 downto 0));
	end component;
	component perder is
	    port (
            rst : in std_logic;
            clk : in std_logic;
            leds : out std_logic_vector(15 downto 0));
	end component;
	
signal ganador,enabl, empate: std_logic;
signal jugador: std_logic_vector(1 downto 0); --salida
signal aleat: std_logic_vector(1 downto 0); --salida
signal conectar_salida_bram1,conectar_salida_bram2:std_logic_vector(1 downto 0);
signal ceros: std_logic_vector(1 downto 0) ;
signal gana: std_logic_vector(15 downto 0);
signal pierde: std_logic_vector(15 downto 0);
signal sec_led: std_logic;
signal comparar: std_logic;

begin
    ceros <= (others => '0');
    MOD_DISPLAYS: displays port map (rst => rst,
                clk => clk,   
                digito_0 =>conectar_salida_bram1,
                digito_1 =>conectar_salida_bram2,
                digito_2 => ceros,
                digito_3 => ceros,
                aleatorio => aleat,
                display => salida_display, 
                display_enable =>salida_display_enable
    );
    MOD_REGISTRO: registro port map(rst, clk, boton, opcion, jugador);
    MOD_ALEATORIO: generadorAleatorio port map(clk,rst,enabl,aleat); 
    MOD_COMPARADOR: comparador port map(jugador, aleat,comparar,ganador,empate); 
    MOD_GANA: ganar port map (rst, clk,leds=>gana);
    MOD_PIERDE: perder port map(rst, clk, leds=>pierde);
    maquina<=aleat;
    SECUENCIAS: process(sec_led)
    begin
        if(sec_led='0') then
            leds<=pierde;
        else
            leds<=gana;
        end if;
    end process SECUENCIAS;
    
    SINCRONO: process (clk, rst)
    begin
        if rising_edge(clk) then
            if (rst='1') then
                ESTADO<=S0; 
            else
                ESTADO<=SIG_ESTADO;
            end if;
        end if;
    end process SINCRONO;
    COMBINACIONAL: process(ESTADO,boton,ganador)
    begin
        
        case ESTADO is 
        when S0=>
             if(boton='1') then --Cargas load
                SIG_ESTADO<=S1;
             else
                SIG_ESTADO<=S0;
             end if;
        when S1=>
            --enter<='1'; --metemos nuestra eleccion
            if(boton='1') then --Cargas load
                SIG_ESTADO<=S2;
             else
                SIG_ESTADO<=S1;
             end if;
        when S2=>
             --enter<='0';
             enabl<='1';  --genera numAleatorio
             if(boton='1') then --Cargas load
                SIG_ESTADO<=S3;
             else
                SIG_ESTADO<=S2;
             end if;
        when S3=>
             enabl<='0';
             comparar<='1'; --obtenemos ganador con el comparador
             if ganador='0' then  --Si gana la maquina
                sec_led<='0';
             elsif(ganador='1') then
                sec_led<='1';
             end if;
             SIG_ESTADO<=S1;
       end case;
    end process COMBINACIONAL;
end Behavioral;
