if (me.args() > 1)
{
    Machine.add("config.ck:"+me.arg(1));
}
else
{
    Machine.add("config.ck");
}
Machine.add("broadcasters/FFT_AUTO_CORR.ck");
Machine.add("broadcasters/PITCH_ESTIMATE.ck");
Machine.add("broadcasters/RMS_TRACKER.ck");

if (me.args() > 0)
{
    Machine.add("client.ck:"+me.arg(0));
    Machine.add("synth.ck:"+me.arg(0));
}
else
{
    Machine.add("client.ck");
    Machine.add("synth.ck");
}
