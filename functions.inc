# -----------------------------------------
# - Gestionnaire de certifications        -
# - (c) Libre En Poche                    -
# - Licence GPL-v3 (voir fichier licence) -
# -----------------------------------------

loadConfig () {
    if [ ! -f ./config ]
    then
        echo -e "\nMerci de creer le fichier config avant de poursuive (voir config.in pour cela).\n"
        exit 1
    fi
    source ./config
}

echoName () {
    echo -e "\nGestionnaire de certification v ${VERSION}\n(c)${ANNEE}, Libre En Poche - GPLv3"
}

testRootCert () {
    if [ ! -f "${TRUSTEDANDCRLDIR}/trusted.pem" ]
    then
        echo -e "\nCreation de l'autorite racine avant de poursuivre (avec mkroot)..."
        ./mkroot > /dev/null 2>&1
        if [ ! -f "${TRUSTEDANDCRLDIR}/trusted.pem" ]
        then
            echo -e "\nEchec de la creation de l'autorite racine. Abandon.\n"
            exit 1
        fi
    fi
}

testIfRootCert () {
    if [ ! -f "${TRUSTEDANDCRLDIR}/trusted.pem" ]
    then
        echo -e "\nPas d'autorite racine. Abandon.\n"
        exit 1
    fi
}

testSubdomainCert () {
    SUBD=$@
    OUSUBD=subdomain-${SUBD}
    
    if [ ! -f "${SUBDIR}/certs/${OUSUBD}.pem" ]
    then
        echo -e "\nCreation du sous-domaine avant de poursuivre (avec mksubdomain)..."
        ./mksubdomain ${SUBD} > /dev/null 2>&1
        if [ ! -f ""${SUBDIR}/certs/${OUSUBD}.pem"" ]
        then
            echo -e "\nEchec de la creation du sous-domaine. Abandon.\n"
            exit 1
        fi
    fi
}

testIfSubdomainCert () {
    SUBD=$@
    OUSUBD=subdomain-${SUBD}

    if [ ! -f "${SUBDIR}/certs/${OUSUBD}.pem" ]
    then
        echo -e "\nPas de sous-domaine. Abandon.\n"
        exit 1
    fi
}

testIfCert () {
    CERTFILE=$@

    if [ ! -f "${CERTFILE}" ]
    then
        echo -e "\nPas de certificat. Abandon.\n"
        exit 1
    fi
}

testIfCertCSRevoked () {
    C=$@
    grep "CN=${C}$" ${SUBDIR}/index.txt | grep "^V" > /dev/null
    if [ $? -ne 0 ]
    then
        echo -e "\nCertificat client/server revoque... Abandon.\n"
        exit 1
    fi
}

testIfCertSubRevoked () {
    C=$@
    grep "CN=subdomain-${C}$" ${ROOTCRTDIR}/index.txt | grep "^V" > /dev/null
    if [ $? -ne 0 ]
    then
        echo -e "\nCertificat sous-domaine revoque... Abandon.\n"
        exit 1
    fi
}

