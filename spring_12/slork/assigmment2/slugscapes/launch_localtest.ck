Machine.add("config.ck:local");
Machine.add("server.ck");
Machine.add("broadcasters/FFT_AUTO_CORR.ck");
Machine.add("broadcasters/PITCH_ESTIMATE.ck");
Machine.add("broadcasters/RMS_TRACKER.ck");
if (me.args() > 0)
{
    Machine.add("synth.ck:"+me.arg(0));
}
else
{
    Machine.add("synth.ck");
}
Machine.add("client.ck");
