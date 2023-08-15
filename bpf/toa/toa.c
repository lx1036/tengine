

// @see https://lore.kernel.org/bpf/20200730205736.3354304-1-kafai@fb.com/


SEC("sockops/estab")
int estab(struct bpf_sock_ops *skops)
{
	int true_val = 1;

	switch (skops->op) {
	case BPF_SOCK_OPS_TCP_LISTEN_CB:
		bpf_setsockopt(skops, SOL_TCP, TCP_SAVE_SYN,
			       &true_val, sizeof(true_val));
		set_hdr_cb_flags(skops, BPF_SOCK_OPS_STATE_CB_FLAG);
		break;
	case BPF_SOCK_OPS_TCP_CONNECT_CB:
		set_hdr_cb_flags(skops, 0);
		break;
	case BPF_SOCK_OPS_PARSE_HDR_OPT_CB:
		return handle_parse_hdr(skops);
	case BPF_SOCK_OPS_HDR_OPT_LEN_CB:
		return handle_hdr_opt_len(skops);
	case BPF_SOCK_OPS_WRITE_HDR_OPT_CB:
		return handle_write_hdr_opt(skops);
	case BPF_SOCK_OPS_PASSIVE_ESTABLISHED_CB:
		return handle_passive_estab(skops);
	case BPF_SOCK_OPS_ACTIVE_ESTABLISHED_CB:
		return handle_active_estab(skops);
	}

	return CG_OK;
}

char _license[] SEC("license") = "GPL";