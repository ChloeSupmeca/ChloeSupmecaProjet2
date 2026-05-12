if (c == BIOLOGIQUE) { // ici le dentiste jette le plateau biologique

    if (d->g.porte_gants && d->mains.i == AUCUN_INSTR) {
        action_jeter_gants(p);
    } else {
        action_vider_plateau_biologique(p);
    }

    return;
}
