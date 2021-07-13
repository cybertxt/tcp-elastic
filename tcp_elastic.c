#include <linux/module.h>
#include <net/tcp.h>

#define ELASTIC_SCALE 6
#define ELASTIC_UNIT (1 << ELASTIC_SCALE)

struct elastic {
	u32	rtt_max;
	u32	rtt_curr;
};

static void elastic_init(struct sock *sk)
{
	struct elastic *ca = inet_csk_ca(sk);
	ca->rtt_max = 0;
	ca->rtt_curr = 1;
}

static void elastic_cong_avoid(struct sock *sk, u32 ack, u32 acked)
{
	struct tcp_sock *tp = tcp_sk(sk);
	struct elastic *ca = inet_csk_ca(sk);

	if (!tcp_is_cwnd_limited(sk))
		return;

	if (tcp_in_slow_start(tp))
		tcp_slow_start(tp, acked);
	else {
		u64 wwf64 = int_sqrt(tp->snd_cwnd*int_pow(ELASTIC_UNIT,2)*ca->rtt_max/ca->rtt_curr);
		u32 wwf = wwf64 >> ELASTIC_SCALE;
		tcp_cong_avoid_ai(tp, tp->snd_cwnd, wwf);
	}
}

static void elastic_update_rtt(struct sock *sk, const struct ack_sample *sample)
{
	struct elastic *ca = inet_csk_ca(sk);
	ca->rtt_curr = sample->rtt_us + 1;
	if (ca->rtt_curr > ca->rtt_max) {
		ca->rtt_max = ca->rtt_curr;
	}
}

static void elastic_event(struct sock *sk, enum tcp_ca_event event)
{
	struct elastic *ca = inet_csk_ca(sk);
	if (event == CA_EVENT_LOSS) {
		ca->rtt_max = 0;
	}
}

static struct tcp_congestion_ops tcp_elastic __read_mostly = {
	.name		= "elastic",
	.owner		= THIS_MODULE,
	.init		= elastic_init,
	.ssthresh	= tcp_reno_ssthresh,
	.undo_cwnd	= tcp_reno_undo_cwnd,
	.cong_avoid	= elastic_cong_avoid,
	.pkts_acked	= elastic_update_rtt,
	.cwnd_event	= elastic_event
};

static int __init elastic_register(void)
{
	BUILD_BUG_ON(sizeof(struct elastic) > ICSK_CA_PRIV_SIZE);
	return tcp_register_congestion_control(&tcp_elastic);
}

static void __exit elastic_unregister(void)
{
	tcp_unregister_congestion_control(&tcp_elastic);
}

module_init(elastic_register);
module_exit(elastic_unregister);

MODULE_AUTHOR("Xie Tao <cxxtao@gmail.com>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Elastic TCP");
