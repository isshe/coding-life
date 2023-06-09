
```
S_visit
Perl_sv_clear
sv_free
```

```c
typedef enum {
        SVt_NULL,	/* 0 */
        /* BIND was here, before INVLIST replaced it.  */
        SVt_IV,		/* 1 */
        SVt_NV,		/* 2 */
        /* RV was here, before it was merged with IV.  */
        SVt_PV,		/* 3 */
        SVt_INVLIST,	/* 4, implemented as a PV */
        SVt_PVIV,	/* 5 */
        SVt_PVNV,	/* 6 */
        SVt_PVMG,	/* 7 */
        SVt_REGEXP,	/* 8 */
        /* PVBM was here, before BIND replaced it.  */
        SVt_PVGV,	/* 9 */
        SVt_PVLV,	/* 10 */
        SVt_PVAV,	/* 11 */
        SVt_PVHV,	/* 12 */
        SVt_PVCV,	/* 13 */
        SVt_PVFM,	/* 14 */
        SVt_PVIO,	/* 15 */
                        /* 16-31: Unused, though one should be reserved for a
                         * freed sv, if the other 3 bits below the flags ones
                         * get allocated */
        SVt_LAST	/* keep last in enum. used to size arrays */
} svtype;
```

C<SVt_PVMG> represents a "normal" scalar (not a typeglob, regular expression,
or delegate).  Since most scalars do not need all the internal fields of a
PVMG, we save memory by allocating smaller structs when possible.
All the other types are just simpler forms of C<SVt_PVMG>, with fewer internal fields.
C<SVt_NULL> can only hold undef.
C<SVt_IV> can hold undef, an integer, or a reference.
(C<SVt_RV> is an alias for C<SVt_IV>, which exists for backward compatibility.)
C<SVt_NV> can hold any of those or a double.
C<SVt_PV> can only hold C<undef> or a string.
C<SVt_PVIV> is a superset of C<SVt_PV> and C<SVt_IV>.
C<SVt_PVNV> is similar.
C<SVt_PVMG> can hold anything C<SVt_PVNV> can hold, but it
can, but does not have to, be blessed or magical.
