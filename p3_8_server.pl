#! /usr/bin/perl
use strict;
use warnings FATAL => 'all';
use Socket;
use English;

my $SOCK_PATH = "$ENV{'HOME'}/socket/lab5_socket_perl";
my $start_time = time();
my %info = (
    time => 0,
    pid  => $PID,
    uid  => $UID,
    gid  => (split (/ /, $GID))[0],
    load1 => 0,
    load2 => 0,
    load3 => 0
);

sub update {
    $info{time} = time() - $start_time;
    my @load = split(/load average: /, qx(uptime));
    @load = split(/, /, $load[1]);
    $info{load1} = $load[0];
    $info{load2} = $load[1];
    $info{load3} = $load[2];
}

sub get_info {
    return (
        sprintf("PID: %li, GID: %li, UID: %li\n", $info{pid}, $info{gid}, $info{uid})
    .   sprintf("Server working %li seconds\n ", $info{time})
    .   sprintf("Average load system time for 1 minute: %.3lf,\n\t 5 minutes: %.3lf,\n\t 15 minutes: %.3lf\n",
            $info{load1},
            $info{load2},
            $info{load3})
    );
}

unlink $SOCK_PATH;
socket(my $server, PF_UNIX, SOCK_STREAM, 0)
    or die "Error in socket: $!";
bind($server, sockaddr_un($SOCK_PATH))
    or die "Can't create server: $!";
listen($server, 10)
    or die "Error in listen: $!";
while (1) {
    update();
    accept(my $client, $server);
    print $client get_info();
}