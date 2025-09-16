LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.numeric_std.ALL;

entity ganar is
	port (
		rst : in std_logic;
		clk : in std_logic;
		leds : out std_logic_vector(15 downto 0));
end ganar;

architecture behavioral of ganar is

	signal leds_aux : std_logic_vector(15 downto 0);

begin
	process (clk, rst, leds_aux)
	begin
		if rst = '1' then
			leds_aux <= "1100110011001100";
		elsif rising_edge(clk) then
			if leds_aux = "0011001100110011" then
				leds_aux <= "1100110011001100";
			else
				leds_aux <= "0011001100110011";
			end if;
		end if;
	end process;

	leds <= leds_aux;

end architecture behavioral;