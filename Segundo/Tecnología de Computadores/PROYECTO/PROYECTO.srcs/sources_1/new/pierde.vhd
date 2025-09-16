LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.numeric_std.ALL;

entity perder is
	port (
		rst : in std_logic;
		clk : in std_logic;
		leds : out std_logic_vector(15 downto 0));
end perder;

architecture behavioral of perder is

	signal leds_aux : std_logic_vector(15 downto 0);

begin


	process (clk, rst, leds_aux)
	begin
		if rst = '1' then
			leds_aux <= (OTHERS => '0');
		elsif rising_edge(clk) then
			if leds_aux = "0000000000000000" then
				leds_aux <= "0000000000000000"; --1111111111111111
			else
				leds_aux <= "1111111111111111"; --0000000000000000
			end if;
		end if;
	end process;

	leds <= leds_aux;

end architecture behavioral;