// Used for testing, do not use it as an example
#include <iostream>

#include "qpp.h"
#include "experimental/experimental.h"

int main() {
    using namespace qpp;
    using namespace qpp::experimental;

    /////////// qubit teleportation ///////////
    QCircuitDescription teleport_qubit{3, 2, 2, "qubit teleportation"};
    // set the qubit 0 to a random state
    cmat U = randU(2);
    teleport_qubit.gate(U, 0);

    // set the MES between qudits 1 and 2
    teleport_qubit.gate(gt.H, 1);
    teleport_qubit.gate(gt.CNOT, 1, 2);

    // perform the Bell measurement between qudits 0 and 1
    teleport_qubit.gate(gt.CNOT, 0, 1);
    teleport_qubit.gate(gt.H, 0);
    teleport_qubit.measureZ(0, 0);
    teleport_qubit.measureZ(1, 1);

    // apply the classical controls
    teleport_qubit.cCTRL(gt.X, 1, 2);
    teleport_qubit.cCTRL(gt.Z, 0, 2);

    QCircuit qc_teleport_qubit{teleport_qubit};
    qc_teleport_qubit.run();
    std::cout << qc_teleport_qubit << '\n';

    ket psi_initial = U * 0_ket;
    ket psi_final = qc_teleport_qubit.get_psi();
    std::cout << "teleported state:\n";
    std::cout << disp(psi_final) << '\n';
    std::cout << "norm difference: " << norm(psi_final - psi_initial) << "\n\n";

    /////////// qudit teleportation ///////////
    idx d = 5;
    QCircuitDescription tele_qudit{3, 2, d, "qudit teleportation"};
    // set the qubit 0 to a random state
    U = randU(d);
    tele_qudit.gate(U, 0);

    // set the MES between qudits 1 and 2
    tele_qudit.gate(gt.Fd(d), 1);
    tele_qudit.CTRL(gt.Xd(d), 1, 2);

    // perform the Bell measurement between qudits 0 and 1
    tele_qudit.CTRL(adjoint(gt.Xd(d)), 0, 1);
    tele_qudit.gate(adjoint(gt.Fd(d)), 0);

    // perform the measurements
    tele_qudit.measureZ(0, 0);
    tele_qudit.measureZ(1, 1);

    // apply the classical controls
    tele_qudit.cCTRL(adjoint(gt.Xd(d)), 1, 2);
    tele_qudit.cCTRL(gt.Zd(d), 0, 2);

    QCircuit qc_tele_qudit{tele_qudit};
    qc_tele_qudit.run();
    std::cout << qc_tele_qudit << '\n';

    psi_initial = U * mket({0}, d);
    psi_final = qc_tele_qudit.get_psi();
    std::cout << "teleported state:\n";
    std::cout << disp(psi_final) << '\n';
    std::cout << "norm difference: " << norm(psi_final - psi_initial) << "\n\n";

    /////////// more testing ///////////

    std::cout << "more testing\n";
    QCircuitDescription qcd{3, 3};
    qcd.gate_fan(gt.H);

    qcd.measureV(gt.Id(4), {0, 2}, 0);
    qcd.measureZ(1, 2);

    QCircuit qc{qcd};
    qc.run();
    std::cout << qc << '\n';
    std::cout << disp(qc.get_psi()) << '\n';
}
