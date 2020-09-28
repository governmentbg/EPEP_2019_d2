// TUser

    // TODO? like CAC print_user()
    public string Print(Connection conn)
    {
        TCitizen citizen = new TCitizen();

        citizen.ucn = ucn;
        citizen.ucnType = TUCNType.CitizenUCN;
        citizen.Get(conn, null);
        return String.Format("{0} {1} {2} {3}", citizen.name, citizen.reName, citizen.family, citizen.reFamily).Replace("  ", " ");
    }
