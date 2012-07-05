



for my $line (<STDIN>) {
  if ($line =~ m/eth0:\s(\d+)/) {
    print $1;
    last;
  }
}
