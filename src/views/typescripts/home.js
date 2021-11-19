"use strict";
function hideAllTypeSetting(components) {
    for (let component of components) {
        if (component !== null) {
            component.visible = false;
        }
    }
}
function networkSelectToggle(currentIndex, components, control = null) {
    if (components === null) {
        return;
    }
    let hostLabel = components["hostLabel"];
    let hostText = components["hostText"];
    let pathLabel = components["pathLabel"];
    let pathText = components["pathText"];
    let typeLabel = components["typeLabel"];
    let typeSelect = components["typeSelect"];
    let quicSecurityLabel = components["quicSecurityLabel"];
    let quicSecuritySelect = components["quicSecuritySelect"];
    hideAllTypeSetting([hostLabel, hostText, pathLabel, pathText, typeLabel, typeSelect, quicSecurityLabel, quicSecuritySelect]);
    switch (currentIndex) {
        case 0: // tcp
            break;
        case 1: // ws
            hostLabel.visible = true;
            hostLabel.text = "Host";
            hostText.visible = true;
            pathLabel.visible = true;
            pathLabel.text = "Path";
            pathText.visible = true;
            break;
        case 2: // grpc
            pathLabel.visible = true;
            pathLabel.text = "Service Name";
            pathText.visible = true;
            break;
        case 3: // quic
            typeLabel.visible = true;
            typeSelect.visible = true;
            quicSecurityLabel.visible = true;
            quicSecuritySelect.visible = true;
            pathLabel.visible = true;
            pathLabel.text = "Key";
            pathText.visible = true;
            break;
        default:
            break;
    }
    if (control !== null) {
        control.configChanged();
    }
}
function visibleChangeToggle(visible, components, model = null) {
    if (!visible) {
        return;
    }
    let alterIDText = components["alterIDText"];
    let securitySelect = components["securitySelect"];
    let networkSelect = components["networkSelect"];
    let pathText = components["pathText"];
    let hostText = components["hostText"];
    let typeSelect = components["typeSelect"];
    let quicSecuritySelect = components["quicSecuritySelect"];
    let tlsEnableSelect = components["tlsEnableSelect"];
    let sniText = components["sniText"];
    if (model !== null && model.raw !== null) {
        let raw = JSON.parse(model.raw);
        if (raw.hasOwnProperty("protocol") && raw["protocol"] !== "vmess") {
            return;
        }
        if (raw.hasOwnProperty("settings")) {
            let vmess = null;
            if (raw["settings"].hasOwnProperty("vmess")) {
                vmess = raw["settings"]["vmess"]["vnext"];
            }
            else {
                vmess = raw["settings"]["vnext"];
            }
            if (vmess !== null && Object.keys(vmess).length > 0) {
                let server = vmess[0];
                if (server.hasOwnProperty("users") && Object.keys(server["users"]).length > 0) {
                    let user = server["users"][0];
                    if (user.hasOwnProperty("alterId") && alterIDText !== null) {
                        alterIDText.text = user["alterId"];
                    }
                    if (user.hasOwnProperty("security") && securitySelect !== null) {
                        securitySelect.currentIndex = securitySelect.find(user["security"]);
                    }
                }
            }
        }
        if (!raw.hasOwnProperty("streamSettings")) {
            return;
        }
        if (raw["streamSettings"] !== null) {
            let streamSettings = raw["streamSettings"];
            if (streamSettings.hasOwnProperty("network") && networkSelect !== null) {
                let network = streamSettings["network"];
                networkSelect.currentIndex = networkSelect.find(network);
                switch (network) {
                    case "ws":
                        if (streamSettings.hasOwnProperty("wsSettings")) {
                            let wsSettings = streamSettings["wsSettings"];
                            if (wsSettings.hasOwnProperty("path") && pathText !== null) {
                                pathText.text = wsSettings["path"];
                            }
                            if (wsSettings.hasOwnProperty("headers")
                                && wsSettings["headers"].hasOwnProperty("Host") && hostText !== null) {
                                hostText.text = wsSettings["headers"]["Host"];
                            }
                        }
                        break;
                    case "grpc":
                        if (streamSettings.hasOwnProperty("grpcSettings") && pathText !== null) {
                            let grpcSettings = streamSettings["grpcSettings"];
                            if (grpcSettings.hasOwnProperty("serviceName")) {
                                pathText.text = grpcSettings["serviceName"];
                            }
                        }
                        break;
                    case "quic":
                        if (streamSettings.hasOwnProperty("quicSettings") && typeSelect !== null) {
                            let quicSettings = streamSettings["quicSettings"];
                            if (quicSettings.hasOwnProperty("header")
                                && quicSettings["header"].hasOwnProperty("type")) {
                                typeSelect.currentIndex = typeSelect.find(quicSettings["header"]["type"]);
                            }
                            if (quicSettings.hasOwnProperty("key") && pathText !== null) {
                                pathText.text = quicSettings["key"];
                            }
                            if (quicSettings.hasOwnProperty("security") && quicSecuritySelect !== null) {
                                quicSecuritySelect.currentIndex = quicSecuritySelect.find(quicSettings["security"]);
                            }
                        }
                        break;
                    default:
                        break;
                }
            }
            if (streamSettings.hasOwnProperty("security") && tlsEnableSelect !== null) {
                if (streamSettings["security"] === "tls") {
                    tlsEnableSelect.checked = true;
                }
            }
            if (streamSettings.hasOwnProperty("tlsSettings") &&
                streamSettings["tlsSettings"].hasOwnProperty("serverName")
                && sniText !== null) {
                sniText.text = streamSettings["tlsSettings"]["serverName"];
            }
        }
    }
}
function displayProtocolText(protocol) {
    switch (protocol) {
        case 0:
            return "vmess";
        case 1:
            return "shadowsocks";
        case 2:
            return "trojan";
        case 4:
            return "raw";
        case 5:
            return "scheme";
        case 6:
        default:
            return "unknown";
    }
}
function isCurrentNode(nodes, groupID, nodeID, core) {
    let result = false;
    do {
        if (nodes === null || core === null)
            break;
        if (nodes.currentGroupID === null || nodes.currentNodeID === null || core.isRunning === null)
            break;
        if (nodes.currentGroupID === groupID
            && nodes.currentNodeID === Number(nodeID)
            && core.isRunning) {
            result = true;
        }
    } while (false);
    return result;
}
function getQRCode(nodes, model, config) {
    let name = nodes.getQRCode(model.nodeID);
    if (name === "") {
        return "qrc:/misc/icons/" + config.iconStyle + "/repo_qr_code.svg";
    }
    else {
        return "image://acrossImageProvider/" + name;
    }
}
