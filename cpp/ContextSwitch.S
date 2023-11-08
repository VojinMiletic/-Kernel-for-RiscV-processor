.global _ZN3TCB13ContextSwitchEPNS_7ContextES1_

_ZN3TCB13ContextSwitchEPNS_7ContextES1_:
    sd ra, 0 * 8(a0)
    sd sp, 1 * 8(a0)

    ld ra, 0 * 8(a1)
    ld sp, 1 * 8(a1)

    ret