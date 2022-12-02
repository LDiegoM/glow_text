function restart() {
    executeCall("/restart", "POST").then((updOk) => {
        if (updOk == true) {
            openModal("Done", "glow_text is restarting");
        }
    });
}
