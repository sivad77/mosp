#!/usr/bin/perl -w
use Time::HiRes qw(usleep ualarm gettimeofday tv_interval);
$args = shift @ARGV;

foreach $file (@ARGV) {
	($name = $file) =~ s/^.*\///; 
	print "$name ";
	
	#time execution
	$t0 = [gettimeofday];
	@result = `./mosp $args $file`;
	$t1 = [gettimeofday];
	
	# get rid of letters 
	s/^[a-zA-Z: ]*// foreach @result;
	
	# get rid of new lines
	chomp foreach @result;
	
	$time = 1000 *(tv_interval $t0, $t1);
	print "\t $result[2] \t $result[6] \t $time  \n";	
}
