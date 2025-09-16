library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity comparador is
    Port(E1, E2: IN std_logic_vector(1 downto 0);
        comp: IN std_logic;
        S,Empate: OUT std_logic
    );
end comparador;

architecture Behavioral of comparador is

begin
    process (E1, E2) 
    begin 
        Empate<='0';
        if(comp='1') then
            if(E1="00" AND E2="01") then --jugador piedra, máquina papel
                S <= '0'; --gana máquina
            elsif(E1="00" AND E2="10") then --jugador piedra, máquina tijeras
                S <= '1'; --gana jugador
            elsif(E1="01" AND E2="00") then --jugador papel, máquina piedra
                S <= '1'; --gana jugador
            elsif(E1="01" AND E2="10") then --jugador papel, máquina tijeras
                S <= '0'; --gana máquina
            elsif(E1="10" AND E2="00") then --jugador tijeras, máquina piedra
                S <= '0'; --gana máquina
            elsif(E1="10" AND E2="01") then --jugador tijeras, máquina papel
                S <= '1'; --gana jugador
            else 
                Empate <= '1';
            end if;
        end if;
    end process;

end Behavioral;
