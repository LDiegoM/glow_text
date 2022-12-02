function setMessage() {
    const body = getMessageBody();

    executeCall("/message", "PUT", body).then((updOk) => {
        if (updOk == true) {
            openModal("Done", "New message was set");
        }
    });
}

function setAnimation() {
    const body = getAnimationBody();

    executeCall("/animation", "PUT", body).then((updOk) => {
        if (updOk == true) {
            openModal("Done", "New animation parameters were set");
        }
    });
}

function getMessageBody() {
    const msg = document.getElementById("msg").value;
    var body = '{"message":"' + msg + '"}';
    return body;
}

function getAnimationBody() {
    const speed = document.getElementById("speed").value;
    const pause = document.getElementById("pause").value;
    var body = '{"speed":' + speed + ',"pause":' + pause + '}';
    return body;
}
