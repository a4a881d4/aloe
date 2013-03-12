
modules:
{

	coder:
	{
		binary="modrep_osld/liblte_cfi_coding.so";	
		mopts=8;
		variables=(
			{name="cfi";value=2}
		);
	};

	scrambling:
	{
		binary="modrep_osld/liblte_scrambling.so";	
		mopts=8;
		variables=(
			{name="subframe";value=0},{name="cell_gr";value=101},{name="cell_sec";value=2},{name="channel";value=1}
		);
	};

	modulator:
	{
		binary="modrep_osld/libgen_modulator.so";	
		mopts=8;
		variables=(
			{name="modulation";value=2}
		);
	};
		
};


interfaces:
(
	{src="coder";dest="scrambling"},
	{src="scrambling";dest="modulator"},
	{src="modulator";dest="_output"}
);
